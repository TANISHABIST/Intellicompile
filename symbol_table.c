#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create a new symbol table */
SymbolTable* create_symbol_table(int size, SymbolTable* parent) {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    table->size = size;
    table->count = 0;
    table->scope_level = parent ? parent->scope_level + 1 : 0;
    table->parent = parent;
    table->child = NULL;
    
    table->buckets = (Symbol**)calloc(size, sizeof(Symbol*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    
    return table;
}

/* Free symbol table */
void free_symbol_table(SymbolTable* table) {
    if (!table) return;
    
    /* Free all symbols */
    for (int i = 0; i < table->size; i++) {
        Symbol* symbol = table->buckets[i];
        while (symbol) {
            Symbol* next = symbol->next;
            free_symbol(symbol);
            symbol = next;
        }
    }
    
    free(table->buckets);
    free(table);
}

/* Create a new symbol */
Symbol* create_symbol(const char* name, SymbolType symbol_type, DataType data_type, int line, int col) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!symbol) return NULL;
    
    symbol->name = strdup(name);
    symbol->symbol_type = symbol_type;
    symbol->data_type = data_type;
    symbol->line_number = line;
    symbol->column_number = col;
    symbol->is_defined = 0;
    symbol->is_used = 0;
    
    /* Initialize function-specific fields */
    symbol->parameters = NULL;
    symbol->param_count = 0;
    symbol->return_type = TYPE_VOID;
    
    /* Initialize array-specific fields */
    symbol->array_size = 0;
    symbol->element_type = TYPE_UNKNOWN;
    
    /* Initialize struct-specific fields */
    symbol->members = NULL;
    symbol->member_count = 0;
    
    symbol->scope_level = 0;
    symbol->next = NULL;
    
    return symbol;
}

/* Free a symbol */
void free_symbol(Symbol* symbol) {
    if (!symbol) return;
    
    if (symbol->name) free(symbol->name);
    if (symbol->parameters) {
        for (int i = 0; i < symbol->param_count; i++) {
            free_symbol(symbol->parameters[i]);
        }
        free(symbol->parameters);
    }
    if (symbol->members) {
        for (int i = 0; i < symbol->member_count; i++) {
            free_symbol(&symbol->members[i]);
        }
        free(symbol->members);
    }
    
    free(symbol);
}

/* Hash function for symbol table */
int hash_function(const char* name, int table_size) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash % table_size;
}

/* Look up symbol in current and parent scopes */
Symbol* lookup_symbol(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    SymbolTable* current = table;
    while (current) {
        Symbol* symbol = lookup_symbol_in_scope(current, name);
        if (symbol) return symbol;
        current = current->parent;
    }
    
    return NULL;
}

/* Look up symbol only in current scope */
Symbol* lookup_symbol_in_scope(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    int index = hash_function(name, table->size);
    Symbol* symbol = table->buckets[index];
    
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    
    return NULL;
}

/* Insert symbol into table */
int insert_symbol(SymbolTable* table, Symbol* symbol) {
    if (!table || !symbol) return 0;
    
    /* Check if symbol already exists in current scope */
    if (lookup_symbol_in_scope(table, symbol->name)) {
        return 0;  // Symbol already exists
    }
    
    int index = hash_function(symbol->name, table->size);
    symbol->next = table->buckets[index];
    table->buckets[index] = symbol;
    symbol->scope_level = table->scope_level;
    
    table->count++;
    return 1;
}

/* Enter new scope */
SymbolTable* enter_scope(SymbolTable* table) {
    if (!table) return NULL;
    
    SymbolTable* new_scope = create_symbol_table(101, table);
    if (!new_scope) return NULL;
    
    table->child = new_scope;
    return new_scope;
}

/* Exit current scope */
SymbolTable* exit_scope(SymbolTable* table) {
    if (!table) return NULL;
    
    SymbolTable* parent = table->parent;
    if (parent) {
        parent->child = NULL;
    }
    
    free_symbol_table(table);
    return parent;
}

/* Check if types are compatible */
int is_compatible_types(DataType type1, DataType type2) {
    if (type1 == type2) return 1;
    
    /* Allow implicit conversion from int to float */
    if (type1 == TYPE_INT && type2 == TYPE_FLOAT) return 1;
    if (type1 == TYPE_FLOAT && type2 == TYPE_INT) return 1;
    
    /* Allow char to int conversion */
    if (type1 == TYPE_CHAR && type2 == TYPE_INT) return 1;
    if (type1 == TYPE_INT && type2 == TYPE_CHAR) return 1;
    
    return 0;
}

/* Get result type of binary operation */
DataType get_result_type(DataType type1, DataType type2, const char* operator) {
    /* Logical operations always return int (boolean) */
    if (strcmp(operator, "==") == 0 || strcmp(operator, "!=") == 0 ||
        strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
        strcmp(operator, "<=") == 0 || strcmp(operator, ">=") == 0 ||
        strcmp(operator, "&&") == 0 || strcmp(operator, "||") == 0 ||
        strcmp(operator, "!") == 0) {
        return TYPE_INT;
    }
    
    /* Arithmetic operations */
    if (type1 == TYPE_FLOAT || type2 == TYPE_FLOAT) {
        return TYPE_FLOAT;
    }
    
    return TYPE_INT;
}

/* Convert data type to string */
const char* data_type_to_string(DataType type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_CHAR: return "char";
        case TYPE_VOID: return "void";
        case TYPE_ARRAY: return "array";
        case TYPE_STRUCT: return "struct";
        case TYPE_FUNCTION: return "function";
        default: return "unknown";
    }
}

/* Convert symbol type to string */
const char* symbol_type_to_string(SymbolType type) {
    switch (type) {
        case SYMBOL_VARIABLE: return "variable";
        case SYMBOL_FUNCTION: return "function";
        case SYMBOL_PARAMETER: return "parameter";
        case SYMBOL_TYPE: return "type";
        case SYMBOL_STRUCT: return "struct";
        default: return "unknown";
    }
}

/* Add semantic error */
void add_semantic_error(SemanticError** errors, const char* message, int line, int col, int error_type, const char* suggestion) {
    SemanticError* error = (SemanticError*)malloc(sizeof(SemanticError));
    if (!error) return;
    
    error->message = strdup(message);
    error->line_number = line;
    error->column_number = col;
    error->error_type = error_type;
    error->suggestion = suggestion ? strdup(suggestion) : NULL;
    error->next = *errors;
    *errors = error;
}

/* Print semantic errors */
void print_semantic_errors(SemanticError* errors) {
    if (!errors) {
        printf("No semantic errors found.\n");
        return;
    }
    
    int error_count = 0;
    int warning_count = 0;
    
    printf("\nSemantic Analysis Results:\n");
    printf("========================\n");
    
    SemanticError* current = errors;
    while (current) {
        const char* type_str = current->error_type == 0 ? "Warning" : 
                              current->error_type == 1 ? "Error" : "Info";
        
        printf("%s at line %d, column %d: %s\n", 
               type_str, current->line_number, current->column_number, current->message);
        
        if (current->suggestion) {
            printf("  Suggestion: %s\n", current->suggestion);
        }
        
        if (current->error_type == 1) error_count++;
        else if (current->error_type == 0) warning_count++;
        
        current = current->next;
    }
    
    printf("\nSummary: %d errors, %d warnings\n", error_count, warning_count);
}

/* Free semantic errors */
void free_semantic_errors(SemanticError* errors) {
    while (errors) {
        SemanticError* next = errors->next;
        if (errors->message) free(errors->message);
        if (errors->suggestion) free(errors->suggestion);
        free(errors);
        errors = next;
    }
}