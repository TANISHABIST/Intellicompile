#ifndef TOKEN_H
#define TOKEN_H

/* Token types enumeration */
typedef enum {
    /* Keywords */
    TOKEN_INT = 100,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_VOID,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_DO,
    TOKEN_RETURN,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_DEFAULT,
    TOKEN_SIZEOF,
    TOKEN_STRUCT,
    TOKEN_TYPEDEF,
    
    /* Operators */
    TOKEN_PLUS = 200,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_ASSIGN,
    TOKEN_PLUS_ASSIGN,
    TOKEN_MINUS_ASSIGN,
    TOKEN_MULTIPLY_ASSIGN,
    TOKEN_DIVIDE_ASSIGN,
    TOKEN_MODULO_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_OR,
    TOKEN_LOGICAL_NOT,
    TOKEN_BITWISE_AND,
    TOKEN_BITWISE_OR,
    TOKEN_BITWISE_XOR,
    TOKEN_BITWISE_NOT,
    TOKEN_LEFT_SHIFT,
    TOKEN_RIGHT_SHIFT,
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_ARROW,
    TOKEN_DOT,
    
    /* Delimiters */
    TOKEN_SEMICOLON = 300,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    
    /* Literals */
    TOKEN_INTEGER_LITERAL = 400,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    
    /* Identifiers and Special */
    TOKEN_IDENTIFIER = 500,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char* lexeme;
    int line;
    int column;
    int length;
} Token;

/* Function declarations */
const char* token_type_to_string(TokenType type);
void print_token(Token* token);

#endif