%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "symbol_table.h"

int line_number = 1;
int column_number = 1;
Token* token_stream = NULL;
int token_count = 0;
int token_capacity = 100;

extern int detect_potential_error(const char* lexeme, int line, int col);
extern char* suggest_fix(const char* lexeme);

void add_token_to_stream(TokenType type, const char* lexeme, int line, int col);
%}

/* Regular Expressions */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?[fF]?
STRING      \"([^\"\\\n]|\\.|\\\n)*\"
CHAR        \'([^\'\\\n]|\\.|\\\n)\'
WHITESPACE  [ \t\r]+
NEWLINE     \n

/* Start conditions */
%x COMMENT

%%

/* Keywords */
"int"           { add_token_to_stream(TOKEN_INT, yytext, line_number, column_number); }
"float"         { add_token_to_stream(TOKEN_FLOAT, yytext, line_number, column_number); }
"char"          { add_token_to_stream(TOKEN_CHAR, yytext, line_number, column_number); }
"void"          { add_token_to_stream(TOKEN_VOID, yytext, line_number, column_number); }
"if"            { add_token_to_stream(TOKEN_IF, yytext, line_number, column_number); }
"else"          { add_token_to_stream(TOKEN_ELSE, yytext, line_number, column_number); }
"while"         { add_token_to_stream(TOKEN_WHILE, yytext, line_number, column_number); }
"for"           { add_token_to_stream(TOKEN_FOR, yytext, line_number, column_number); }
"do"            { add_token_to_stream(TOKEN_DO, yytext, line_number, column_number); }
"return"        { add_token_to_stream(TOKEN_RETURN, yytext, line_number, column_number); }
"break"         { add_token_to_stream(TOKEN_BREAK, yytext, line_number, column_number); }
"continue"      { add_token_to_stream(TOKEN_CONTINUE, yytext, line_number, column_number); }
"switch"        { add_token_to_stream(TOKEN_SWITCH, yytext, line_number, column_number); }
"case"          { add_token_to_stream(TOKEN_CASE, yytext, line_number, column_number); }
"default"       { add_token_to_stream(TOKEN_DEFAULT, yytext, line_number, column_number); }
"sizeof"        { add_token_to_stream(TOKEN_SIZEOF, yytext, line_number, column_number); }
"struct"        { add_token_to_stream(TOKEN_STRUCT, yytext, line_number, column_number); }
"typedef"       { add_token_to_stream(TOKEN_TYPEDEF, yytext, line_number, column_number); }

/* Operators */
"+"             { add_token_to_stream(TOKEN_PLUS, yytext, line_number, column_number); }
"-"             { add_token_to_stream(TOKEN_MINUS, yytext, line_number, column_number); }
"*"             { add_token_to_stream(TOKEN_MULTIPLY, yytext, line_number, column_number); }
"/"             { add_token_to_stream(TOKEN_DIVIDE, yytext, line_number, column_number); }
"%"             { add_token_to_stream(TOKEN_MODULO, yytext, line_number, column_number); }
"="             { add_token_to_stream(TOKEN_ASSIGN, yytext, line_number, column_number); }
"+="            { add_token_to_stream(TOKEN_PLUS_ASSIGN, yytext, line_number, column_number); }
"-="            { add_token_to_stream(TOKEN_MINUS_ASSIGN, yytext, line_number, column_number); }
"*="            { add_token_to_stream(TOKEN_MULTIPLY_ASSIGN, yytext, line_number, column_number); }
"/="            { add_token_to_stream(TOKEN_DIVIDE_ASSIGN, yytext, line_number, column_number); }
"%="            { add_token_to_stream(TOKEN_MODULO_ASSIGN, yytext, line_number, column_number); }
"=="            { add_token_to_stream(TOKEN_EQUAL, yytext, line_number, column_number); }
"!="            { add_token_to_stream(TOKEN_NOT_EQUAL, yytext, line_number, column_number); }
"<"             { add_token_to_stream(TOKEN_LESS_THAN, yytext, line_number, column_number); }
">"             { add_token_to_stream(TOKEN_GREATER_THAN, yytext, line_number, column_number); }
"<="            { add_token_to_stream(TOKEN_LESS_EQUAL, yytext, line_number, column_number); }
">="            { add_token_to_stream(TOKEN_GREATER_EQUAL, yytext, line_number, column_number); }
"&&"            { add_token_to_stream(TOKEN_LOGICAL_AND, yytext, line_number, column_number); }
"||"            { add_token_to_stream(TOKEN_LOGICAL_OR, yytext, line_number, column_number); }
"!"             { add_token_to_stream(TOKEN_LOGICAL_NOT, yytext, line_number, column_number); }
"&"             { add_token_to_stream(TOKEN_BITWISE_AND, yytext, line_number, column_number); }
"|"             { add_token_to_stream(TOKEN_BITWISE_OR, yytext, line_number, column_number); }
"^"             { add_token_to_stream(TOKEN_BITWISE_XOR, yytext, line_number, column_number); }
"~"             { add_token_to_stream(TOKEN_BITWISE_NOT, yytext, line_number, column_number); }
"<<"            { add_token_to_stream(TOKEN_LEFT_SHIFT, yytext, line_number, column_number); }
">>"            { add_token_to_stream(TOKEN_RIGHT_SHIFT, yytext, line_number, column_number); }
"++"            { add_token_to_stream(TOKEN_INCREMENT, yytext, line_number, column_number); }
"--"            { add_token_to_stream(TOKEN_DECREMENT, yytext, line_number, column_number); }
"->"            { add_token_to_stream(TOKEN_ARROW, yytext, line_number, column_number); }
"."             { add_token_to_stream(TOKEN_DOT, yytext, line_number, column_number); }

/* Delimiters */
";"             { add_token_to_stream(TOKEN_SEMICOLON, yytext, line_number, column_number); }
","             { add_token_to_stream(TOKEN_COMMA, yytext, line_number, column_number); }
":"             { add_token_to_stream(TOKEN_COLON, yytext, line_number, column_number); }
"("             { add_token_to_stream(TOKEN_LPAREN, yytext, line_number, column_number); }
")"             { add_token_to_stream(TOKEN_RPAREN, yytext, line_number, column_number); }
"{"             { add_token_to_stream(TOKEN_LBRACE, yytext, line_number, column_number); }
"}"             { add_token_to_stream(TOKEN_RBRACE, yytext, line_number, column_number); }
"["             { add_token_to_stream(TOKEN_LBRACKET, yytext, line_number, column_number); }
"]"             { add_token_to_stream(TOKEN_RBRACKET, yytext, line_number, column_number); }

/* Literals */
{INTEGER}       { add_token_to_stream(TOKEN_INTEGER_LITERAL, yytext, line_number, column_number); }
{FLOAT}         { add_token_to_stream(TOKEN_FLOAT_LITERAL, yytext, line_number, column_number); }
{STRING}        { add_token_to_stream(TOKEN_STRING_LITERAL, yytext, line_number, column_number); }
{CHAR}          { add_token_to_stream(TOKEN_CHAR_LITERAL, yytext, line_number, column_number); }

/* Identifiers */
{ID}            { 
                    add_token_to_stream(TOKEN_IDENTIFIER, yytext, line_number, column_number);
                    
                    /* AI-enhanced error detection */
                    if (detect_potential_error(yytext, line_number, column_number)) {
                        char* suggestion = suggest_fix(yytext);
                        fprintf(stderr, "AI Warning at line %d, col %d: Potential error in identifier '%s'. Suggestion: %s\n", 
                                line_number, column_number, yytext, suggestion);
                        free(suggestion);
                    }
                }

/* Comments */
"//"            { BEGIN(COMMENT); }
<COMMENT>\n     { 
                    BEGIN(INITIAL); 
                    line_number++;
                    column_number = 1;
                }
<COMMENT>.      { /* ignore comment content */ }

"/*"            { 
                    int comment_line = line_number;
                    int comment_col = column_number;
                    int nesting = 1;
                    
                    while (nesting > 0) {
                        int c = input();
                        if (c == EOF) {
                            fprintf(stderr, "Error: Unterminated comment starting at line %d, col %d\n", 
                                    comment_line, comment_col);
                            break;
                        }
                        
                        if (c == '\n') {
                            line_number++;
                            column_number = 1;
                        } else {
                            column_number++;
                        }
                        
                        if (c == '/' && (c = input()) == '*') {
                            nesting++;
                            column_number++;
                        } else if (c == '*' && (c = input()) == '/') {
                            nesting--;
                            column_number++;
                        } else if (c != EOF) {
                            column_number++;
                        }
                    }
                }

/* Whitespace */
{WHITESPACE}    { column_number += yyleng; }
{NEWLINE}       { 
                    line_number++; 
                    column_number = 1; 
                }

/* Invalid tokens - error handling */
.               { 
                    fprintf(stderr, "Error: Invalid token '%s' at line %d, column %d\n", 
                            yytext, line_number, column_number);
                    
                    /* AI-enhanced error suggestion */
                    char* suggestion = suggest_fix(yytext);
                    if (suggestion) {
                        fprintf(stderr, "AI Suggestion: %s\n", suggestion);
                        free(suggestion);
                    }
                    
                    column_number += yyleng;
                }

%%

/* C code section */
int yywrap(void) {
    return 1;
}

void add_token_to_stream(TokenType type, const char* lexeme, int line, int col) {
    /* Initialize token stream if needed */
    if (token_stream == NULL) {
        token_stream = (Token*)malloc(token_capacity * sizeof(Token));
        if (!token_stream) {
            fprintf(stderr, "Error: Memory allocation failed for token stream\n");
            exit(1);
        }
    }
    
    /* Resize if needed */
    if (token_count >= token_capacity) {
        token_capacity *= 2;
        token_stream = (Token*)realloc(token_stream, token_capacity * sizeof(Token));
        if (!token_stream) {
            fprintf(stderr, "Error: Memory reallocation failed for token stream\n");
            exit(1);
        }
    }
    
    /* Add token */
    Token* token = &token_stream[token_count];
    token->type = type;
    token->lexeme = strdup(lexeme);
    token->line = line;
    token->column = col;
    token->length = yyleng;
    
    token_count++;
}

void print_token_stream() {
    printf("Token Stream (%d tokens):\n", token_count);
    printf("=====================================\n");
    
    for (int i = 0; i < token_count; i++) {
        Token* token = &token_stream[i];
        printf("Token[%d]: Type=%d, Lexeme='%s', Line=%d, Col=%d\n", 
               i, token->type, token->lexeme, token->line, token->column);
    }
}

void free_token_stream() {
    if (token_stream) {
        for (int i = 0; i < token_count; i++) {
            free(token_stream[i].lexeme);
        }
        free(token_stream);
        token_stream = NULL;
        token_count = 0;
    }
}

/* Main function for testing */
int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    
    printf("Starting lexical analysis...\n");
    yylex();
    
    printf("\nLexical analysis completed.\n");
    print_token_stream();
    
    free_token_stream();
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return 0;
}