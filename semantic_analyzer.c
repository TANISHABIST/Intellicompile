#include "semantic_analyzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create semantic context */
SemanticContext* create_semantic_context(void) {
    SemanticContext* context = (SemanticContext*)malloc(sizeof(SemanticContext));
    if (!context) return NULL;
    
    context->global_scope = create_symbol_table(101, NULL);
    context->current_scope = context->global_scope;
    context->errors = NULL;
    context->current_function = NULL;
    context->in_loop = 0;
    context->error_count = 0;
    context->warning_count = 0;
    
    return context;
}

/* Free semantic context */
void free_semantic_context(SemanticContext* context) {
    if (!context) return;
    
    free_symbol_table(context->global_scope);
    free_semantic_errors(context->errors);
    free(context);
}

/* Analyze AST */
int analyze_ast(ASTNode* ast, SemanticContext* context) {
    if (!ast || !context) return 0;
    
    analyze_program(ast, context);
    
    return context->error_count == 0;
}

/* Analyze program */
void analyze_program(ASTNode* node, SemanticContext* context) {
    if (!node || node->type != NODE_PROGRAM) return;
    
    for (int i = 0; i < node->child_count; i++) {
        ASTNode* child = node->children[i];
        
        if (child->type == NODE_FUNCTION_DEFINITION) {
            analyze_function_definition(child, context);
        } else if (child->type == NODE_DECLARATION) {
            analyze_declaration(child, context);
        }
    }
}

/* Analyze function definition */
void analyze_function_definition(ASTNode* node, SemanticContext* context) {
    if (!node || node->type != NODE_FUNCTION_DEFINITION) return;
    
    /* Extract function name and return type */
    char* func_name = node->value;
    DataType return_type = TYPE_VOID;
    
    if (node->child_count > 0 && node->children[0]->type == NODE_TYPE) {
        return_type = parse_data_type(node->children[0]->value);
    }
    
    /* Check if function already declared */
    Symbol* existing = lookup_symbol(context->global_scope, func_name);
    if (existing && existing->symbol_type == SYMBOL_FUNCTION) {
        if (existing->is_defined) {
            add_semantic_error(&context->errors, 
                              "Function already defined", 
                              node->line, node->column, 1,
                              "Use a different function name");
            context->error_count++;
        } else {
            /* Function was previously declared, now being defined */
            existing->is_defined = 1;
            context->current_function = existing;
        }
    } else {
        /* Declare new function */
        Symbol* func_symbol = create_symbol(func_name, SYMBOL_FUNCTION, return_type, 
                                          node->line, node->column);
        func_symbol->return_type = return_type;
        func_symbol->is_defined = 1;
        
        if (!insert_symbol(context->global_scope, func_symbol)) {
            add_semantic_error(&context->errors, 
                              "Failed to insert function symbol", 
                              node->line, node->column, 1, NULL);
            context->error_count++;
            free_symbol(func_symbol);
            return;
        }
        
        context->current_function = func_symbol;
    }
    
    /* Enter function scope */
    context->current_scope = enter_scope(context->current_scope);
    
    /* Analyze parameters */
    if (node->child_count > 1 && node->children[1]->type == NODE_PARAM_LIST) {
        ASTNode* param_list = node->children[1];
        for (int i = 0; i < param_list->child_count; i++) {
            analyze_declaration(param_list->children[i], context);
        }
    }
    
    /* Analyze function body */
    if (node->child_count > 2) {
        for (int i = 2; i < node->child_count; i++) {
            if (node->children[i]->type == NODE_COMPOUND_STMT) {
                analyze_statement(node->children[i], context);
            }
        }
    }
    
    /* Exit function scope */
    context->current_scope = exit_scope(context->current_scope);
    context->current_function = NULL;
}

/* Analyze declaration */
void analyze_declaration(ASTNode* node, SemanticContext* context) {
    if (!node || node->type != NODE_DECLARATION) return;
    
    DataType decl_type = TYPE_UNKNOWN;
    
    /* Get declaration type */
    if (node->child_count > 0 && node->children[0]->type == NODE_TYPE) {
        decl_type = parse_data_type(node->children[0]->value);
    }
    
    /* Process each declarator */
    for (int i = 1; i < node->child_count; i++) {
        ASTNode* declarator = node->children[i];
        
        if (declarator->type == NODE_IDENTIFIER) {
            declare_variable(declarator, decl_type, context);
        } else if (declarator->type == NODE_INIT_DECL) {
            /* Variable with initialization */
            if (declarator->child_count > 0) {
                declare_variable(declarator->children[0], decl_type, context);
                
                /* Check initialization expression */
                if (declarator->child_count > 1) {
                    DataType init_type = analyze_expression_type(declarator->children[1], context);
                    if (!is_compatible_types(decl_type, init_type)) {
                        add_semantic_error(&context->errors,
                                          "Type mismatch in variable initialization",
                                          declarator->line, declarator->column, 1,
                                          "Ensure initialization expression matches variable type");
                        context->error_count++;
                    }
                }
            }
        }
    }
}

/* Declare variable */
void declare_variable(ASTNode* node, DataType type, SemanticContext* context) {
    if (!node || node->type != NODE_IDENTIFIER) return;
    
    char* var_name = node->value;
    
    /* Check if variable already declared in current scope */
    Symbol* existing = lookup_symbol_in_scope(context->current_scope, var_name);
    if (existing) {
        add_semantic_error(&context->errors,
                          "Variable already declared in this scope",
                          node->line, node->column, 1,
                          "Use a different variable name");
        context->error_count++;
        return;
    }
    
    /* Create and insert symbol */
    Symbol* var_symbol = create_symbol(var_name, SYMBOL_VARIABLE, type,
                                      node->line, node->column);
    
    if (!insert_symbol(context->current_scope, var_symbol)) {
        add_semantic_error(&context->errors,
                          "Failed to insert variable symbol",
                          node->line, node->column, 1, NULL);
        context->error_count++;
        free_symbol(var_symbol);
        return;
    }
}

/* Analyze statement */
void analyze_statement(ASTNode* node, SemanticContext* context) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_COMPOUND_STMT:
            context->current_scope = enter_scope(context->current_scope);
            for (int i = 0; i < node->child_count; i++) {
                analyze_statement(node->children[i], context);
            }
            context->current_scope = exit_scope(context->current_scope);
            break;
            
        case NODE_EXPRESSION_STMT:
            if (node->child_count > 0) {
                analyze_expression(node->children[0], context);
            }
            break;
            
        case NODE_IF_STMT:
        case NODE_IF_ELSE_STMT:
            if (node->child_count > 0) {
                analyze_expression(node->children[0], context);
                for (int i = 1; i < node->child_count; i++) {
                    analyze_statement(node->children[i], context);
                }
            }
            break;
            
        case NODE_WHILE_STMT:
        case NODE_DO_WHILE_STMT:
            context->in_loop = 1;
            if (node->child_count > 0) {
                analyze_expression(node->children[0], context);
            }
            if (node->child_count > 1) {
                analyze_statement(node->children[1], context);
            }
            context->in_loop = 0;
            break;
            
        case NODE_FOR_STMT:
            context->in_loop = 1;
            for (int i = 0; i < node->child_count; i++) {
                if (i == 0 || i == 1 || i == 2) {
                    analyze_expression(node->children[i], context);
                } else {
                    analyze_statement(node->children[i], context);
                }
            }
            context->in_loop = 0;
            break;
            
        case NODE_RETURN_STMT:
            check_return_statement(node, context);
            break;
            
        case NODE_BREAK_STMT:
        case NODE_CONTINUE_STMT:
            if (!context->in_loop) {
                add_semantic_error(&context->errors,
                                  "Break/continue statement outside loop",
                                  node->line, node->column, 1,
                                  "Move break/continue inside a loop");
                context->error_count++;
            }
            break;
            
        default:
            for (int i = 0; i < node->child_count; i++) {
                analyze_statement(node->children[i], context);
            }
            break;
    }
}

/* Analyze expression */
void analyze_expression(ASTNode* node, SemanticContext* context) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_IDENTIFIER:
            {
                Symbol* symbol = lookup_symbol(context->current_scope, node->value);
                if (!symbol) {
                    add_semantic_error(&context->errors,
                                      "Undeclared variable",
                                      node->line, node->column, 1,
                                      "Declare the variable before using it");
                    context->error_count++;
                } else {
                    symbol->is_used = 1;
                }
            }
            break;
            
        case NODE_ASSIGN_EXPR:
            if (node->child_count >= 2) {
                check_assignment_compatibility(node->children[0], node->children[1], context);
            }
            break;
            
        case NODE_FUNCTION_CALL:
            check_function_call(node, context);
            break;
            
        case NODE_BINARY_EXPR:
            if (node->child_count >= 2) {
                analyze_expression(node->children[0], context);
                analyze_expression(node->children[1], context);
                
                DataType left_type = analyze_expression_type(node->children[0], context);
                DataType right_type = analyze_expression_type(node->children[1], context);
                
                if (!is_compatible_types(left_type, right_type)) {
                    add_semantic_error(&context->errors,
                                      "Type mismatch in binary operation",
                                      node->line, node->column, 1,
                                      "Ensure both operands have compatible types");
                    context->error_count++;
                }
            }
            break;
            
        case NODE_UNARY_EXPR:
            if (node->child_count > 0) {
                analyze_expression(node->children[0], context);
            }
            break;
            
        default:
            for (int i = 0; i < node->child_count; i++) {
                analyze_expression(node->children[i], context);
            }
            break;
    }
}

/* Get expression type */
DataType analyze_expression_type(ASTNode* node, SemanticContext* context) {
    if (!node) return TYPE_UNKNOWN;
    
    switch (node->type) {
        case NODE_IDENTIFIER:
            {
                Symbol* symbol = lookup_symbol(context->current_scope, node->value);
                return symbol ? symbol->data_type : TYPE_UNKNOWN;
            }
            
        case NODE_INT_LITERAL:
            return TYPE_INT;
            
        case NODE_FLOAT_LITERAL:
            return TYPE_FLOAT;
            
        case NODE_CHAR_LITERAL:
            return TYPE_CHAR;
            
        case NODE_STRING_LITERAL:
            return TYPE_ARRAY;  /* String is array of chars */
            
        case NODE_BINARY_EXPR:
            if (node->child_count >= 2) {
                DataType left_type = analyze_expression_type(node->children[0], context);
                DataType right_type = analyze_expression_type(node->children[1], context);
                return get_result_type(left_type, right_type, node->value);
            }
            break;
            
        case NODE_UNARY_EXPR:
            if (node->child_count > 0) {
                return analyze_expression_type(node->children[0], context);
            }
            break;
            
        case NODE_FUNCTION_CALL:
            {
                Symbol* symbol = lookup_symbol(context->current_scope, node->children[0]->value);
                return symbol ? symbol->return_type : TYPE_UNKNOWN;
            }
            
        default:
            break;
    }
    
    return TYPE_UNKNOWN;
}

/* Check assignment compatibility */
void check_assignment_compatibility(ASTNode* left, ASTNode* right, SemanticContext* context) {
    if (!left || !right) return;
    
    DataType left_type = analyze_expression_type(left, context);
    DataType right_type = analyze_expression_type(right, context);
    
    if (!is_compatible_types(left_type, right_type)) {
        add_semantic_error(&context->errors,
                          "Type mismatch in assignment",
                          left->line, left->column, 1,
                          "Ensure right-hand side type is compatible with left-hand side");
        context->error_count++;
    }
}

/* Check function call */
void check_function_call(ASTNode* node, SemanticContext* context) {
    if (!node || node->type != NODE_FUNCTION_CALL || node->child_count == 0) return;
    
    ASTNode* func_name_node = node->children[0];
    Symbol* function = lookup_symbol(context->current_scope, func_name_node->value);
    
    if (!function) {
        add_semantic_error(&context->errors,
                          "Undeclared function",
                          func_name_node->line, func_name_node->column, 1,
                          "Declare the function before calling it");
        context->error_count++;
        return;
    }
    
    if (function->symbol_type != SYMBOL_FUNCTION) {
        add_semantic_error(&context->errors,
                          "Called object is not a function",
                          func_name_node->line, func_name_node->column, 1,
                          "Ensure you are calling a function");
        context->error_count++;
        return;
    }
    
    /* Check parameter count */
    int arg_count = node->child_count - 1;
    if (arg_count != function->param_count) {
        add_semantic_error(&context->errors,
                          "Incorrect number of arguments in function call",
                          func_name_node->line, func_name_node->column, 1,
                          "Provide the correct number of arguments");
        context->error_count++;
        return;
    }
    
    /* Check parameter types */
    for (int i = 0; i < arg_count && i < function->param_count; i++) {
        DataType arg_type = analyze_expression_type(node->children[i + 1], context);
        DataType param_type = function->parameters[i]->data_type;
        
        if (!is_compatible_types(param_type, arg_type)) {
            add_semantic_error(&context->errors,
                              "Type mismatch in function argument",
                              node->children[i + 1]->line, node->children[i + 1]->column, 1,
                              "Ensure argument type matches parameter type");
            context->error_count++;
        }
    }
}

/* Check return statement */
void check_return_statement(ASTNode* node, SemanticContext* context) {
    if (!node || node->type != NODE_RETURN_STMT) return;
    
    if (!context->current_function) {
        add_semantic_error(&context->errors,
                          "Return statement outside function",
                          node->line, node->column, 1,
                          "Remove return statement or place inside function");
        context->error_count++;
        return;
    }
    
    DataType expected_type = context->current_function->return_type;
    
    if (node->child_count > 0) {
        DataType return_type = analyze_expression_type(node->children[0], context);
        
        if (!is_compatible_types(expected_type, return_type)) {
            add_semantic_error(&context->errors,
                              "Return type mismatch",
                              node->line, node->column, 1,
                              "Ensure return value matches function return type");
            context->error_count++;
        }
    } else if (expected_type != TYPE_VOID) {
        add_semantic_error(&context->errors,
                          "Missing return value in non-void function",
                          node->line, node->column, 1,
                          "Add return value or change function return type to void");
        context->error_count++;
    }
}

/* Parse data type from string */
DataType parse_data_type(const char* type_str) {
    if (!type_str) return TYPE_UNKNOWN;
    
    if (strcmp(type_str, "int") == 0) return TYPE_INT;
    if (strcmp(type_str, "float") == 0) return TYPE_FLOAT;
    if (strcmp(type_str, "char") == 0) return TYPE_CHAR;
    if (strcmp(type_str, "void") == 0) return TYPE_VOID;
    
    return TYPE_UNKNOWN;
}

/* AI-enhanced semantic error detection */
int detect_semantic_error_pattern(ASTNode* node, SemanticContext* context) {
    if (!node) return 0;
    
    /* Check for common semantic error patterns */
    if (node->type == NODE_IDENTIFIER) {
        /* Check for common variable name mistakes */
        if (strstr(node->value, "lenght") != NULL) return 85;
        if (strstr(node->value, "widht") != NULL) return 85;
        if (strstr(node->value, "hieght") != NULL) return 85;
    }
    
    return 0;
}

/* Suggest semantic fix */
char* suggest_semantic_fix(ASTNode* node, SemanticContext* context) {
    if (!node) return NULL;
    
    char* suggestion = malloc(200);
    
    if (node->type == NODE_IDENTIFIER) {
        if (strstr(node->value, "lenght") != NULL) {
            snprintf(suggestion, 200, "Did you mean 'length' instead of '%s'?", node->value);
            return suggestion;
        }
        if (strstr(node->value, "widht") != NULL) {
            snprintf(suggestion, 200, "Did you mean 'width' instead of '%s'?", node->value);
            return suggestion;
        }
    }
    
    snprintf(suggestion, 200, "Review the semantic usage near this construct");
    return suggestion;
}