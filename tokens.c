#include "token.h"
#include <stdio.h>

const char* token_type_to_string(TokenType type) 
{
    switch (type) 
    {
        /* Keywords */
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_VOID: return "VOID";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_DO: return "DO";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_SWITCH: return "SWITCH";
        case TOKEN_CASE: return "CASE";
        case TOKEN_DEFAULT: return "DEFAULT";
        case TOKEN_SIZEOF: return "SIZEOF";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_TYPEDEF: return "TYPEDEF";
        
        /* Operators */
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TOKEN_MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TOKEN_MULTIPLY_ASSIGN: return "MULTIPLY_ASSIGN";
        case TOKEN_DIVIDE_ASSIGN: return "DIVIDE_ASSIGN";
        case TOKEN_MODULO_ASSIGN: return "MODULO_ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS_THAN: return "LESS_THAN";
        case TOKEN_GREATER_THAN: return "GREATER_THAN";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_LOGICAL_AND: return "LOGICAL_AND";
        case TOKEN_LOGICAL_OR: return "LOGICAL_OR";
        case TOKEN_LOGICAL_NOT: return "LOGICAL_NOT";
        case TOKEN_BITWISE_AND: return "BITWISE_AND";
        case TOKEN_BITWISE_OR: return "BITWISE_OR";
        case TOKEN_BITWISE_XOR: return "BITWISE_XOR";
        case TOKEN_BITWISE_NOT: return "BITWISE_NOT";
        case TOKEN_LEFT_SHIFT: return "LEFT_SHIFT";
        case TOKEN_RIGHT_SHIFT: return "RIGHT_SHIFT";
        case TOKEN_INCREMENT: return "INCREMENT";
        case TOKEN_DECREMENT: return "DECREMENT";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_DOT: return "DOT";
        
        /* Delimiters */
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_COLON: return "COLON";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        
        /* Literals */
        case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        
        /* Identifiers and Special */
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        
        default: return "UNKNOWN";
    }
}

void print_token(Token* token) 
{
    printf("Token: Type=%s, Lexeme='%s', Line=%d, Col=%d\n",
           token_type_to_string(token->type),
           token->lexeme,
           token->line,
           token->column);
}