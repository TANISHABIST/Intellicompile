#ifndef AST_H
#define AST_H

/* AST node types */
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION_DEFINITION,
    NODE_DECLARATION,
    NODE_TYPE,
    NODE_STRUCT_TYPE,
    NODE_PARAM_DECL,
    NODE_PARAM_LIST,
    NODE_INIT_DECL,
    NODE_IDENTIFIER,
    NODE_ARRAY_DECL,
    NODE_ARRAY_SPEC,
    NODE_INITIALIZER_LIST,
    NODE_COMPOUND_STMT,
    NODE_EMPTY_STMT,
    NODE_IF_STMT,
    NODE_IF_ELSE_STMT,
    NODE_SWITCH_STMT,
    NODE_CASE_STMT,
    NODE_DEFAULT_STMT,
    NODE_WHILE_STMT,
    NODE_DO_WHILE_STMT,
    NODE_FOR_STMT,
    NODE_RETURN_STMT,
    NODE_BREAK_STMT,
    NODE_CONTINUE_STMT,
    NODE_EXPRESSION_STMT,
    NODE_ASSIGN_EXPR,
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_POSTFIX_INC,
    NODE_POSTFIX_DEC,
    NODE_CONDITIONAL_EXPR,
    NODE_COMMA_EXPR,
    NODE_CAST_EXPR,
    NODE_SIZEOF_EXPR,
    NODE_ARRAY_ACCESS,
    NODE_FUNCTION_CALL,
    NODE_MEMBER_ACCESS,
    NODE_PAREN_EXPR,
    NODE_EMPTY_EXPR,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_CHAR_LITERAL,
    NODE_STRING_LITERAL,
    NODE_STRUCT_DECL
} NodeType;

/* AST node structure */
typedef struct ASTNode {
    NodeType type;
    char* value;
    int line;
    int column;
    struct ASTNode** children;
    int child_count;
    struct ASTNode* parent;
} ASTNode;

/* Function declarations */
const char* node_type_to_string(NodeType type);
void print_ast(ASTNode* node, int indent);
void free_ast(ASTNode* node);

#endif