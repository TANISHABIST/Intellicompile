%{
/* C declarations and includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "token.h"
#include "syntax_error_detector.h"

/* External functions from lexer */
extern int yylex(void);
extern int yyparse(void);
extern FILE* yyin;
extern int line_number;
extern int column_number;

/* Error handling */
extern int yyerror(const char* s);

/* Global AST root */
ASTNode* program_ast = NULL;
int syntax_errors = 0;

/* AI-enhanced syntax error detection */
extern int detect_syntax_error_pattern(int token_type, const char* lexeme);
extern char* suggest_syntax_fix(int token_type, const char* lexeme);

/* Function prototypes */
ASTNode* create_ast_node(NodeType type, const char* value, int line, int col);
ASTNode* create_binary_node(NodeType type, ASTNode* left, ASTNode* right, const char* op, int line, int col);
ASTNode* create_unary_node(NodeType type, ASTNode* operand, const char* op, int line, int col);
void add_child(ASTNode* parent, ASTNode* child);
%}

/* Token declarations */
%token TOKEN_INT TOKEN_FLOAT TOKEN_CHAR TOKEN_VOID
%token TOKEN_IF TOKEN_ELSE TOKEN_WHILE TOKEN_FOR TOKEN_DO
%token TOKEN_RETURN TOKEN_BREAK TOKEN_CONTINUE
%token TOKEN_SWITCH TOKEN_CASE TOKEN_DEFAULT TOKEN_SIZEOF TOKEN_STRUCT TOKEN_TYPEDEF
%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULTIPLY TOKEN_DIVIDE TOKEN_MODULO
%token TOKEN_ASSIGN TOKEN_PLUS_ASSIGN TOKEN_MINUS_ASSIGN TOKEN_MULTIPLY_ASSIGN TOKEN_DIVIDE_ASSIGN TOKEN_MODULO_ASSIGN
%token TOKEN_EQUAL TOKEN_NOT_EQUAL TOKEN_LESS_THAN TOKEN_GREATER_THAN TOKEN_LESS_EQUAL TOKEN_GREATER_EQUAL
%token TOKEN_LOGICAL_AND TOKEN_LOGICAL_OR TOKEN_LOGICAL_NOT
%token TOKEN_BITWISE_AND TOKEN_BITWISE_OR TOKEN_BITWISE_XOR TOKEN_BITWISE_NOT TOKEN_LEFT_SHIFT TOKEN_RIGHT_SHIFT
%token TOKEN_INCREMENT TOKEN_DECREMENT TOKEN_ARROW TOKEN_DOT
%token TOKEN_SEMICOLON TOKEN_COMMA TOKEN_COLON
%token TOKEN_LPAREN TOKEN_RPAREN TOKEN_LBRACE TOKEN_RBRACE TOKEN_LBRACKET TOKEN_RBRACKET
%token TOKEN_IDENTIFIER TOKEN_INTEGER_LITERAL TOKEN_FLOAT_LITERAL TOKEN_STRING_LITERAL TOKEN_CHAR_LITERAL
%token TOKEN_INVALID

/* Operator precedence and associativity */
%right TOKEN_ASSIGN TOKEN_PLUS_ASSIGN TOKEN_MINUS_ASSIGN TOKEN_MULTIPLY_ASSIGN TOKEN_DIVIDE_ASSIGN TOKEN_MODULO_ASSIGN
%left TOKEN_LOGICAL_OR
%left TOKEN_LOGICAL_AND
%left TOKEN_BITWISE_OR
%left TOKEN_BITWISE_XOR
%left TOKEN_BITWISE_AND
%left TOKEN_EQUAL TOKEN_NOT_EQUAL
%left TOKEN_LESS_THAN TOKEN_GREATER_THAN TOKEN_LESS_EQUAL TOKEN_GREATER_EQUAL
%left TOKEN_LEFT_SHIFT TOKEN_RIGHT_SHIFT
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULTIPLY TOKEN_DIVIDE TOKEN_MODULO
%right TOKEN_LOGICAL_NOT TOKEN_BITWISE_NOT TOKEN_INCREMENT TOKEN_DECREMENT
%left TOKEN_DOT TOKEN_ARROW
%left TOKEN_LBRACKET TOKEN_LPAREN

/* Start symbol */
%start program

%%

/* Grammar Rules */

program
    : external_declaration_list
        {
            program_ast = create_ast_node(NODE_PROGRAM, "program", line_number, column_number);
            program_ast->children = $1.children;
            program_ast->child_count = $1.child_count;
        }
    | /* empty program */
        {
            program_ast = create_ast_node(NODE_PROGRAM, "program", line_number, column_number);
        }
    ;

external_declaration_list
    : external_declaration
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | external_declaration_list external_declaration
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    ;

external_declaration
    : function_definition
        { $$ = $1; }
    | declaration
        { $$ = $1; }
    ;

function_definition
    : type_specifier TOKEN_IDENTIFIER TOKEN_LPAREN parameter_list_opt TOKEN_RPAREN compound_statement
        {
            $$ = create_ast_node(NODE_FUNCTION_DEFINITION, $2.value, $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
            add_child($$, $5);
        }
    | type_specifier TOKEN_IDENTIFIER TOKEN_LPAREN parameter_list_opt TOKEN_RPAREN declaration_list compound_statement
        {
            $$ = create_ast_node(NODE_FUNCTION_DEFINITION, $2.value, $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
            add_child($$, $5);
            add_child($$, $4);
        }
    ;

type_specifier
    : TOKEN_INT
        { $$ = create_ast_node(NODE_TYPE, "int", line_number, column_number); }
    | TOKEN_FLOAT
        { $$ = create_ast_node(NODE_TYPE, "float", line_number, column_number); }
    | TOKEN_CHAR
        { $$ = create_ast_node(NODE_TYPE, "char", line_number, column_number); }
    | TOKEN_VOID
        { $$ = create_ast_node(NODE_TYPE, "void", line_number, column_number); }
    | struct_specifier
        { $$ = $1; }
    ;

struct_specifier
    : TOKEN_STRUCT TOKEN_IDENTIFIER
        { $$ = create_ast_node(NODE_STRUCT_TYPE, $2.value, $2.line, $2.column); }
    | TOKEN_STRUCT TOKEN_IDENTIFIER TOKEN_LBRACE struct_declaration_list TOKEN_RBRACE
        {
            $$ = create_ast_node(NODE_STRUCT_TYPE, $2.value, $2.line, $2.column);
            add_child($$, $4);
        }
    ;

parameter_list_opt
    : /* empty */
        { $$ = create_ast_node(NODE_PARAM_LIST, "", line_number, column_number); }
    | parameter_list
        { $$ = $1; }
    ;

parameter_list
    : parameter_declaration
        {
            $$ = create_ast_node(NODE_PARAM_LIST, "", line_number, column_number);
            add_child($$, $1);
        }
    | parameter_list TOKEN_COMMA parameter_declaration
        {
            add_child($1, $3);
            $$ = $1;
        }
    ;

parameter_declaration
    : type_specifier TOKEN_IDENTIFIER
        {
            $$ = create_ast_node(NODE_PARAM_DECL, $2.value, $2.line, $2.column);
            add_child($$, $1);
        }
    | type_specifier TOKEN_IDENTIFIER TOKEN_LBRACKET TOKEN_RBRACKET
        {
            $$ = create_ast_node(NODE_PARAM_DECL, $2.value, $2.line, $2.column);
            add_child($$, $1);
            ASTNode* array_spec = create_ast_node(NODE_ARRAY_SPEC, "[]", line_number, column_number);
            add_child($$, array_spec);
        }
    ;

declaration
    : type_specifier init_declarator_list TOKEN_SEMICOLON
        {
            $$ = create_ast_node(NODE_DECLARATION, "", line_number, column_number);
            add_child($$, $1);
            $$->children = $2.children;
            $$->child_count = $2.child_count;
        }
    ;

init_declarator_list
    : init_declarator
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | init_declarator_list TOKEN_COMMA init_declarator
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $3;
        }
    ;

init_declarator
    : declarator
        { $$ = $1; }
    | declarator TOKEN_ASSIGN initializer
        {
            $$ = create_ast_node(NODE_INIT_DECL, "", $1.line, $1.column);
            add_child($$, $1);
            add_child($$, $3);
        }
    ;

declarator
    : TOKEN_IDENTIFIER
        { $$ = create_ast_node(NODE_IDENTIFIER, $1.value, $1.line, $1.column); }
    | TOKEN_IDENTIFIER TOKEN_LBRACKET constant_expression TOKEN_RBRACKET
        {
            $$ = create_ast_node(NODE_ARRAY_DECL, $1.value, $1.line, $1.column);
            add_child($$, $3);
        }
    ;

initializer
    : assignment_expression
        { $$ = $1; }
    | TOKEN_LBRACE initializer_list TOKEN_RBRACE
        {
            $$ = create_ast_node(NODE_INITIALIZER_LIST, "", line_number, column_number);
            $$->children = $2.children;
            $$->child_count = $2.child_count;
        }
    ;

initializer_list
    : initializer
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | initializer_list TOKEN_COMMA initializer
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $3;
        }
    ;

compound_statement
    : TOKEN_LBRACE declaration_list_opt statement_list_opt TOKEN_RBRACE
        {
            $$ = create_ast_node(NODE_COMPOUND_STMT, "", line_number, column_number);
            if ($2.child_count > 0) {
                for (int i = 0; i < $2.child_count; i++) {
                    add_child($$, $2.children[i]);
                }
            }
            if ($3.child_count > 0) {
                for (int i = 0; i < $3.child_count; i++) {
                    add_child($$, $3.children[i]);
                }
            }
        }
    ;

declaration_list_opt
    : /* empty */
        { $$.child_count = 0; $$.children = NULL; }
    | declaration_list
        { $$ = $1; }
    ;

declaration_list
    : declaration
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | declaration_list declaration
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    ;

statement_list_opt
    : /* empty */
        { $$.child_count = 0; $$.children = NULL; }
    | statement_list
        { $$ = $1; }
    ;

statement_list
    : statement
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | statement_list statement
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    ;

statement
    : expression_statement
        { $$ = $1; }
    | compound_statement
        { $$ = $1; }
    | selection_statement
        { $$ = $1; }
    | iteration_statement
        { $$ = $1; }
    | jump_statement
        { $$ = $1; }
    ;

expression_statement
    : TOKEN_SEMICOLON
        { $$ = create_ast_node(NODE_EMPTY_STMT, "", line_number, column_number); }
    | expression TOKEN_SEMICOLON
        { $$ = $1; }
    ;

selection_statement
    : TOKEN_IF TOKEN_LPAREN expression TOKEN_RPAREN statement
        {
            $$ = create_ast_node(NODE_IF_STMT, "if", $1.line, $1.column);
            add_child($$, $3);
            add_child($$, $5);
        }
    | TOKEN_IF TOKEN_LPAREN expression TOKEN_RPAREN statement TOKEN_ELSE statement
        {
            $$ = create_ast_node(NODE_IF_ELSE_STMT, "if-else", $1.line, $1.column);
            add_child($$, $3);
            add_child($$, $5);
            add_child($$, $7);
        }
    | TOKEN_SWITCH TOKEN_LPAREN expression TOKEN_RPAREN TOKEN_LBRACE switch_block TOKEN_RBRACE
        {
            $$ = create_ast_node(NODE_SWITCH_STMT, "switch", $1.line, $1.column);
            add_child($$, $3);
            add_child($$, $6);
        }
    ;

switch_block
    : /* empty */
        { $$.child_count = 0; $$.children = NULL; }
    | switch_block case_statement
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    | switch_block default_statement
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    ;

case_statement
    : TOKEN_CASE constant_expression TOKEN_COLON statement_list
        {
            $$ = create_ast_node(NODE_CASE_STMT, "case", $1.line, $1.column);
            add_child($$, $2);
            if ($4.child_count > 0) {
                for (int i = 0; i < $4.child_count; i++) {
                    add_child($$, $4.children[i]);
                }
            }
        }
    ;

default_statement
    : TOKEN_DEFAULT TOKEN_COLON statement_list
        {
            $$ = create_ast_node(NODE_DEFAULT_STMT, "default", $1.line, $1.column);
            if ($3.child_count > 0) {
                for (int i = 0; i < $3.child_count; i++) {
                    add_child($$, $3.children[i]);
                }
            }
        }
    ;

iteration_statement
    : TOKEN_WHILE TOKEN_LPAREN expression TOKEN_RPAREN statement
        {
            $$ = create_ast_node(NODE_WHILE_STMT, "while", $1.line, $1.column);
            add_child($$, $3);
            add_child($$, $5);
        }
    | TOKEN_DO statement TOKEN_WHILE TOKEN_LPAREN expression TOKEN_RPAREN TOKEN_SEMICOLON
        {
            $$ = create_ast_node(NODE_DO_WHILE_STMT, "do-while", $1.line, $1.column);
            add_child($$, $2);
            add_child($$, $5);
        }
    | TOKEN_FOR TOKEN_LPAREN expression_opt TOKEN_SEMICOLON expression_opt TOKEN_SEMICOLON expression_opt TOKEN_RPAREN statement
        {
            $$ = create_ast_node(NODE_FOR_STMT, "for", $1.line, $1.column);
            add_child($$, $3);
            add_child($$, $5);
            add_child($$, $7);
            add_child($$, $9);
        }
    ;

expression_opt
    : /* empty */
        { $$ = create_ast_node(NODE_EMPTY_EXPR, "", line_number, column_number); }
    | expression
        { $$ = $1; }
    ;

jump_statement
    : TOKEN_RETURN expression_opt TOKEN_SEMICOLON
        {
            $$ = create_ast_node(NODE_RETURN_STMT, "return", $1.line, $1.column);
            add_child($$, $2);
        }
    | TOKEN_BREAK TOKEN_SEMICOLON
        { $$ = create_ast_node(NODE_BREAK_STMT, "break", $1.line, $1.column); }
    | TOKEN_CONTINUE TOKEN_SEMICOLON
        { $$ = create_ast_node(NODE_CONTINUE_STMT, "continue", $1.line, $1.column); }
    ;

expression
    : assignment_expression
        { $$ = $1; }
    | expression TOKEN_COMMA assignment_expression
        {
            $$ = create_ast_node(NODE_COMMA_EXPR, ",", $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
        }
    ;

assignment_expression
    : conditional_expression
        { $$ = $1; }
    | unary_expression assignment_operator assignment_expression
        {
            $$ = create_ast_node(NODE_ASSIGN_EXPR, $2.value, $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
        }
    ;

assignment_operator
    : TOKEN_ASSIGN
        { $$.value = "="; $$.line = line_number; $$.column = column_number; }
    | TOKEN_PLUS_ASSIGN
        { $$.value = "+="; $$.line = line_number; $$.column = column_number; }
    | TOKEN_MINUS_ASSIGN
        { $$.value = "-="; $$.line = line_number; $$.column = column_number; }
    | TOKEN_MULTIPLY_ASSIGN
        { $$.value = "*="; $$.line = line_number; $$.column = column_number; }
    | TOKEN_DIVIDE_ASSIGN
        { $$.value = "/="; $$.line = line_number; $$.column = column_number; }
    | TOKEN_MODULO_ASSIGN
        { $$.value = "%="; $$.line = line_number; $$.column = column_number; }
    ;

conditional_expression
    : logical_or_expression
        { $$ = $1; }
    | logical_or_expression TOKEN_QUESTION expression TOKEN_COLON conditional_expression
        {
            $$ = create_ast_node(NODE_CONDITIONAL_EXPR, "?:", $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
            add_child($$, $5);
        }
    ;

logical_or_expression
    : logical_and_expression
        { $$ = $1; }
    | logical_or_expression TOKEN_LOGICAL_OR logical_and_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "||", $2.line, $2.column);
        }
    ;

logical_and_expression
    : inclusive_or_expression
        { $$ = $1; }
    | logical_and_expression TOKEN_LOGICAL_AND inclusive_or_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "&&", $2.line, $2.column);
        }
    ;

inclusive_or_expression
    : exclusive_or_expression
        { $$ = $1; }
    | inclusive_or_expression TOKEN_BITWISE_OR exclusive_or_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "|", $2.line, $2.column);
        }
    ;

exclusive_or_expression
    : and_expression
        { $$ = $1; }
    | exclusive_or_expression TOKEN_BITWISE_XOR and_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "^", $2.line, $2.column);
        }
    ;

and_expression
    : equality_expression
        { $$ = $1; }
    | and_expression TOKEN_BITWISE_AND equality_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "&", $2.line, $2.column);
        }
    ;

equality_expression
    : relational_expression
        { $$ = $1; }
    | equality_expression TOKEN_EQUAL relational_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "==", $2.line, $2.column);
        }
    | equality_expression TOKEN_NOT_EQUAL relational_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "!=", $2.line, $2.column);
        }
    ;

relational_expression
    : shift_expression
        { $$ = $1; }
    | relational_expression TOKEN_LESS_THAN shift_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "<", $2.line, $2.column);
        }
    | relational_expression TOKEN_GREATER_THAN shift_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, ">", $2.line, $2.column);
        }
    | relational_expression TOKEN_LESS_EQUAL shift_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "<=", $2.line, $2.column);
        }
    | relational_expression TOKEN_GREATER_EQUAL shift_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, ">=", $2.line, $2.column);
        }
    ;

shift_expression
    : additive_expression
        { $$ = $1; }
    | shift_expression TOKEN_LEFT_SHIFT additive_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "<<", $2.line, $2.column);
        }
    | shift_expression TOKEN_RIGHT_SHIFT additive_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, ">>", $2.line, $2.column);
        }
    ;

additive_expression
    : multiplicative_expression
        { $$ = $1; }
    | additive_expression TOKEN_PLUS multiplicative_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "+", $2.line, $2.column);
        }
    | additive_expression TOKEN_MINUS multiplicative_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "-", $2.line, $2.column);
        }
    ;

multiplicative_expression
    : cast_expression
        { $$ = $1; }
    | multiplicative_expression TOKEN_MULTIPLY cast_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "*", $2.line, $2.column);
        }
    | multiplicative_expression TOKEN_DIVIDE cast_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "/", $2.line, $2.column);
        }
    | multiplicative_expression TOKEN_MODULO cast_expression
        {
            $$ = create_binary_node(NODE_BINARY_EXPR, $1, $3, "%", $2.line, $2.column);
        }
    ;

cast_expression
    : unary_expression
        { $$ = $1; }
    | TOKEN_LPAREN type_specifier TOKEN_RPAREN cast_expression
        {
            $$ = create_ast_node(NODE_CAST_EXPR, "", $1.line, $1.column);
            add_child($$, $2);
            add_child($$, $4);
        }
    ;

unary_expression
    : postfix_expression
        { $$ = $1; }
    | TOKEN_INCREMENT unary_expression
        {
            $$ = create_unary_node(NODE_UNARY_EXPR, $2, "++", $1.line, $1.column);
        }
    | TOKEN_DECREMENT unary_expression
        {
            $$ = create_unary_node(NODE_UNARY_EXPR, $2, "--", $1.line, $1.column);
        }
    | unary_operator cast_expression
        {
            $$ = create_unary_node(NODE_UNARY_EXPR, $2, $1.value, $1.line, $1.column);
        }
    | TOKEN_SIZEOF TOKEN_LPAREN type_specifier TOKEN_RPAREN
        {
            $$ = create_ast_node(NODE_SIZEOF_EXPR, "sizeof", $1.line, $1.column);
            add_child($$, $3);
        }
    ;

unary_operator
    : TOKEN_PLUS
        { $$.value = "+"; $$.line = line_number; $$.column = column_number; }
    | TOKEN_MINUS
        { $$.value = "-"; $$.line = line_number; $$.column = column_number; }
    | TOKEN_LOGICAL_NOT
        { $$.value = "!"; $$.line = line_number; $$.column = column_number; }
    | TOKEN_BITWISE_NOT
        { $$.value = "~"; $$.line = line_number; $$.column = column_number; }
    ;

postfix_expression
    : primary_expression
        { $$ = $1; }
    | postfix_expression TOKEN_LBRACKET expression TOKEN_RBRACKET
        {
            $$ = create_ast_node(NODE_ARRAY_ACCESS, "[]", $2.line, $2.column);
            add_child($$, $1);
            add_child($$, $3);
        }
    | postfix_expression TOKEN_LPAREN argument_expression_list_opt TOKEN_RPAREN
        {
            $$ = create_ast_node(NODE_FUNCTION_CALL, "()", $2.line, $2.column);
            add_child($$, $1);
            if ($3.child_count > 0) {
                for (int i = 0; i < $3.child_count; i++) {
                    add_child($$, $3.children[i]);
                }
            }
        }
    | postfix_expression TOKEN_DOT TOKEN_IDENTIFIER
        {
            $$ = create_ast_node(NODE_MEMBER_ACCESS, ".", $2.line, $2.column);
            add_child($$, $1);
            add_child($$, create_ast_node(NODE_IDENTIFIER, $3.value, $3.line, $3.column));
        }
    | postfix_expression TOKEN_ARROW TOKEN_IDENTIFIER
        {
            $$ = create_ast_node(NODE_MEMBER_ACCESS, "->", $2.line, $2.column);
            add_child($$, $1);
            add_child($$, create_ast_node(NODE_IDENTIFIER, $3.value, $3.line, $3.column));
        }
    | postfix_expression TOKEN_INCREMENT
        {
            $$ = create_unary_node(NODE_POSTFIX_INC, $1, "++", $2.line, $2.column);
        }
    | postfix_expression TOKEN_DECREMENT
        {
            $$ = create_unary_node(NODE_POSTFIX_DEC, $1, "--", $2.line, $2.column);
        }
    ;

primary_expression
    : TOKEN_IDENTIFIER
        { $$ = create_ast_node(NODE_IDENTIFIER, $1.value, $1.line, $1.column); }
    | constant
        { $$ = $1; }
    | TOKEN_STRING_LITERAL
        { $$ = create_ast_node(NODE_STRING_LITERAL, $1.value, $1.line, $1.column); }
    | TOKEN_LPAREN expression TOKEN_RPAREN
        {
            $$ = create_ast_node(NODE_PAREN_EXPR, "()", $1.line, $1.column);
            add_child($$, $2);
        }
    ;

constant
    : TOKEN_INTEGER_LITERAL
        { $$ = create_ast_node(NODE_INT_LITERAL, $1.value, $1.line, $1.column); }
    | TOKEN_FLOAT_LITERAL
        { $$ = create_ast_node(NODE_FLOAT_LITERAL, $1.value, $1.line, $1.column); }
    | TOKEN_CHAR_LITERAL
        { $$ = create_ast_node(NODE_CHAR_LITERAL, $1.value, $1.line, $1.column); }
    ;

argument_expression_list_opt
    : /* empty */
        { $$.child_count = 0; $$.children = NULL; }
    | argument_expression_list
        { $$ = $1; }
    ;

argument_expression_list
    : assignment_expression
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | argument_expression_list TOKEN_COMMA assignment_expression
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $3;
        }
    ;

constant_expression
    : conditional_expression
        { $$ = $1; }
    ;

struct_declaration_list
    : struct_declaration
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | struct_declaration_list struct_declaration
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $2;
        }
    ;

struct_declaration
    : type_specifier declarator_list TOKEN_SEMICOLON
        {
            $$ = create_ast_node(NODE_STRUCT_DECL, "", line_number, column_number);
            add_child($$, $1);
            $$->children = $2.children;
            $$->child_count = $2.child_count;
        }
    ;

declarator_list
    : declarator
        {
            $$.children = (ASTNode**)malloc(sizeof(ASTNode*));
            $$.children[0] = $1;
            $$.child_count = 1;
        }
    | declarator_list TOKEN_COMMA declarator
        {
            $$.child_count = $1.child_count + 1;
            $$.children = (ASTNode**)realloc($1.children, $$.child_count * sizeof(ASTNode*));
            $$.children[$$.child_count - 1] = $3;
        }
    ;

%%

/* C code section */

int yyerror(const char* s) {
    syntax_errors++;
    fprintf(stderr, "Syntax Error at line %d, column %d: %s\n", line_number, column_number, s);
    
    /* AI-enhanced error detection */
    int lookahead = yylex();
    if (detect_syntax_error_pattern(lookahead, yytext)) {
        char* suggestion = suggest_syntax_fix(lookahead, yytext);
        fprintf(stderr, "AI Suggestion: %s\n", suggestion);
        free(suggestion);
    }
    
    return 0;
}

/* AST node creation functions */
ASTNode* create_ast_node(NodeType type, const char* value, int line, int col) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->line = line;
    node->column = col;
    node->children = NULL;
    node->child_count = 0;
    node->parent = NULL;
    return node;
}

ASTNode* create_binary_node(NodeType type, ASTNode* left, ASTNode* right, const char* op, int line, int col) {
    ASTNode* node = create_ast_node(type, op, line, col);
    add_child(node, left);
    add_child(node, right);
    return node;
}

ASTNode* create_unary_node(NodeType type, ASTNode* operand, const char* op, int line, int col) {
    ASTNode* node = create_ast_node(type, op, line, col);
    add_child(node, operand);
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    
    parent->child_count++;
    parent->children = (ASTNode**)realloc(parent->children, parent->child_count * sizeof(ASTNode*));
    parent->children[parent->child_count - 1] = child;
    child->parent = parent;
}

/* AST printing functions */
void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s", node_type_to_string(node->type));
    if (node->value) printf(" (%s)", node->value);
    printf(" [%d:%d]\n", node->line, node->column);
    
    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], indent + 1);
    }
}

void free_ast(ASTNode* node) {
    if (!node) return;
    
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }
    
    if (node->value) free(node->value);
    if (node->children) free(node->children);
    free(node);
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
    
    printf("Starting syntax analysis...\n");
    int parse_result = yyparse();
    
    if (parse_result == 0 && syntax_errors == 0) {
        printf("\nSyntax analysis completed successfully.\n");
        printf("Abstract Syntax Tree:\n");
        printf("=====================\n");
        print_ast(program_ast, 0);
    } else {
        printf("\nSyntax analysis failed with %d errors.\n", syntax_errors);
    }
    
    if (program_ast) {
        free_ast(program_ast);
    }
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return syntax_errors > 0 ? 1 : 0;
}