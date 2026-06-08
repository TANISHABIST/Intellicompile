#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

/* Syntax error patterns for AI detection */
typedef struct {
    int token_type;
    const char* pattern;
    const char* suggestion;
    int confidence;
} SyntaxErrorPattern;

static SyntaxErrorPattern syntax_error_patterns[] = {
    {TOKEN_SEMICOLON, "missing_semicolon", "Add ';' at the end of the statement", 95},
    {TOKEN_RBRACE, "missing_brace", "Add '}' to close the block", 90},
    {TOKEN_RPAREN, "missing_paren", "Add ')' to close the expression", 90},
    {TOKEN_RBRACKET, "missing_bracket", "Add ']' to close the array access", 85},
    {TOKEN_IDENTIFIER, "undeclared_variable", "Check if variable is declared", 80},
    {TOKEN_INVALID, "invalid_token", "Remove or replace invalid token", 70},
    {0, NULL, NULL, 0}
};

int detect_syntax_error_pattern(int token_type, const char* lexeme) {
    if (!lexeme) return 0;
    
    /* Check against known error patterns */
    for (int i = 0; syntax_error_patterns[i].pattern != NULL; i++) {
        if (token_type == syntax_error_patterns[i].token_type) {
            return syntax_error_patterns[i].confidence;
        }
    }
    
    /* Check for common syntax mistakes */
    if (token_type == TOKEN_IDENTIFIER) {
        if (strcmp(lexeme, "then") == 0) return 95;
        if (strcmp(lexeme, "begin") == 0) return 95;
        if (strcmp(lexeme, "end") == 0) return 95;
        if (strcmp(lexeme, "procedure") == 0) return 90;
    }
    
    return 0;
}

char* suggest_syntax_fix(int token_type, const char* lexeme) {
    if (!lexeme) return NULL;
    
    /* Check against known error patterns */
    for (int i = 0; syntax_error_patterns[i].pattern != NULL; i++) {
        if (token_type == syntax_error_patterns[i].token_type) {
            return strdup(syntax_error_patterns[i].suggestion);
        }
    }
    
    /* Generate specific suggestions */
    char* suggestion = NULL;
    
    if (token_type == TOKEN_IDENTIFIER) {
        if (strcmp(lexeme, "then") == 0) {
            suggestion = strdup("Replace 'then' with '{' for C-style syntax");
        } else if (strcmp(lexeme, "begin") == 0) {
            suggestion = strdup("Replace 'begin' with '{' for C-style syntax");
        } else if (strcmp(lexeme, "end") == 0) {
            suggestion = strdup("Replace 'end' with '}' for C-style syntax");
        } else if (strcmp(lexeme, "procedure") == 0) {
            suggestion = strdup("Replace 'procedure' with 'function' or 'void'");
        }
    }
    
    if (!suggestion) {
        suggestion = malloc(200);
        if (token_type == TOKEN_SEMICOLON) {
            snprintf(suggestion, 200, "Check for missing semicolon before this point");
        } else if (token_type == TOKEN_RBRACE) {
            snprintf(suggestion, 200, "Check for missing opening brace '{'");
        } else if (token_type == TOKEN_RPAREN) {
            snprintf(suggestion, 200, "Check for missing opening parenthesis '('");
        } else {
            snprintf(suggestion, 200, "Review syntax near token '%s'", lexeme);
        }
    }
    
    return suggestion;
}