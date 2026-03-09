#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbol types */
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER,
    SYMBOL_TYPE,
    SYMBOL_STRUCT
} SymbolType;

/* Data types */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_FUNCTION,
    TYPE_UNKNOWN
} DataType;

/* Symbol structure */
typedef struct Symbol {
    char* name;
    SymbolType symbol_type;
    DataType data_type;
    int line_number;
    int column_number;
    int is_defined;
    int is_used;
    
    /* For functions */
    struct Symbol** parameters;
    int param_count;
    DataType return_type;
    
    /* For arrays */
    int array_size;
    DataType element_type;
    
    /* For structs */
    struct Symbol* members;
    int member_count;
    
    /* Scope information */
    int scope_level;
    
    /* Next symbol in hash table chain */
    struct Symbol* next;
} Symbol;

/* Symbol table structure */
typedef struct SymbolTable {
    Symbol** buckets;
    int size;
    int count;
    int scope_level;
    struct SymbolTable* parent;
    struct SymbolTable* child;
} SymbolTable;

/* Error information */
typedef struct SemanticError {
    char* message;
    int line_number;
    int column_number;
    char* suggestion;
    int error_type;  // 0=warning, 1=error, 2=info
    struct SemanticError* next;
} SemanticError;

/* Function declarations */
SymbolTable* create_symbol_table(int size, SymbolTable* parent);
void free_symbol_table(SymbolTable* table);
Symbol* create_symbol(const char* name, SymbolType type, DataType data_type, int line, int col);
void free_symbol(Symbol* symbol);
int hash_function(const char* name, int table_size);
Symbol* lookup_symbol(SymbolTable* table, const char* name);
Symbol* lookup_symbol_in_scope(SymbolTable* table, const char* name);
int insert_symbol(SymbolTable* table, Symbol* symbol);
SymbolTable* enter_scope(SymbolTable* table);
SymbolTable* exit_scope(SymbolTable* table);

/* Type checking functions */
int is_compatible_types(DataType type1, DataType type2);
DataType get_result_type(DataType type1, DataType type2, const char* operator);
const char* data_type_to_string(DataType type);
const char* symbol_type_to_string(SymbolType type);

/* Error reporting */
void add_semantic_error(SemanticError** errors, const char* message, int line, int col, int error_type, const char* suggestion);
void print_semantic_errors(SemanticError* errors);
void free_semantic_errors(SemanticError* errors);

#endif