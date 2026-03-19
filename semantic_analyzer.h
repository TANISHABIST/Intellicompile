#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"

/* Semantic analyzer context */
typedef struct SemanticContext {
    SymbolTable* current_scope;
    SymbolTable* global_scope;
    SemanticError* errors;
    Symbol* current_function;
    int in_loop;
    int error_count;
    int warning_count;
} SemanticContext;

/* Function declarations */
SemanticContext* create_semantic_context(void);
void free_semantic_context(SemanticContext* context);
int analyze_ast(ASTNode* ast, SemanticContext* context);

/* Analysis functions */
void analyze_program(ASTNode* node, SemanticContext* context);
void analyze_function_definition(ASTNode* node, SemanticContext* context);
void analyze_declaration(ASTNode* node, SemanticContext* context);
void analyze_statement(ASTNode* node, SemanticContext* context);
void analyze_expression(ASTNode* node, SemanticContext* context);

/* Type checking functions */
DataType analyze_expression_type(ASTNode* node, SemanticContext* context);
void check_assignment_compatibility(ASTNode* left, ASTNode* right, SemanticContext* context);
void check_function_call(ASTNode* node, SemanticContext* context);
void check_return_statement(ASTNode* node, SemanticContext* context);

/* Symbol table functions */
void declare_variable(ASTNode* node, DataType type, SemanticContext* context);
void declare_function(ASTNode* node, SemanticContext* context);
void declare_parameter(ASTNode* node, DataType type, SemanticContext* context);

/* AI-enhanced semantic error detection */
int detect_semantic_error_pattern(ASTNode* node, SemanticContext* context);
char* suggest_semantic_fix(ASTNode* node, SemanticContext* context);

#endif