#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* node_type_to_string(NodeType type) 
{
    switch (type) 
    {
        case NODE_PROGRAM: return "PROGRAM";
        case NODE_FUNCTION_DEFINITION: return "FUNCTION_DEFINITION";
        case NODE_DECLARATION: return "DECLARATION";
        case NODE_TYPE: return "TYPE";
        case NODE_STRUCT_TYPE: return "STRUCT_TYPE";
        case NODE_PARAM_DECL: return "PARAM_DECL";
        case NODE_PARAM_LIST: return "PARAM_LIST";
        case NODE_INIT_DECL: return "INIT_DECL";
        case NODE_IDENTIFIER: return "IDENTIFIER";
        case NODE_ARRAY_DECL: return "ARRAY_DECL";
        case NODE_ARRAY_SPEC: return "ARRAY_SPEC";
        case NODE_INITIALIZER_LIST: return "INITIALIZER_LIST";
        case NODE_COMPOUND_STMT: return "COMPOUND_STMT";
        case NODE_EMPTY_STMT: return "EMPTY_STMT";
        case NODE_IF_STMT: return "IF_STMT";
        case NODE_IF_ELSE_STMT: return "IF_ELSE_STMT";
        case NODE_SWITCH_STMT: return "SWITCH_STMT";
        case NODE_CASE_STMT: return "CASE_STMT";
        case NODE_DEFAULT_STMT: return "DEFAULT_STMT";
        case NODE_WHILE_STMT: return "WHILE_STMT";
        case NODE_DO_WHILE_STMT: return "DO_WHILE_STMT";
        case NODE_FOR_STMT: return "FOR_STMT";
        case NODE_RETURN_STMT: return "RETURN_STMT";
        case NODE_BREAK_STMT: return "BREAK_STMT";
        case NODE_CONTINUE_STMT: return "CONTINUE_STMT";
        case NODE_EXPRESSION_STMT: return "EXPRESSION_STMT";
        case NODE_ASSIGN_EXPR: return "ASSIGN_EXPR";
        case NODE_BINARY_EXPR: return "BINARY_EXPR";
        case NODE_UNARY_EXPR: return "UNARY_EXPR";
        case NODE_POSTFIX_INC: return "POSTFIX_INC";
        case NODE_POSTFIX_DEC: return "POSTFIX_DEC";
        case NODE_CONDITIONAL_EXPR: return "CONDITIONAL_EXPR";
        case NODE_COMMA_EXPR: return "COMMA_EXPR";
        case NODE_CAST_EXPR: return "CAST_EXPR";
        case NODE_SIZEOF_EXPR: return "SIZEOF_EXPR";
        case NODE_ARRAY_ACCESS: return "ARRAY_ACCESS";
        case NODE_FUNCTION_CALL: return "FUNCTION_CALL";
        case NODE_MEMBER_ACCESS: return "MEMBER_ACCESS";
        case NODE_PAREN_EXPR: return "PAREN_EXPR";
        case NODE_EMPTY_EXPR: return "EMPTY_EXPR";
        case NODE_INT_LITERAL: return "INT_LITERAL";
        case NODE_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case NODE_CHAR_LITERAL: return "CHAR_LITERAL";
        case NODE_STRING_LITERAL: return "STRING_LITERAL";
        case NODE_STRUCT_DECL: return "STRUCT_DECL";
        default: return "UNKNOWN";
    }
}