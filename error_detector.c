#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Common error patterns for AI detection */
typedef struct 
{
    const char* pattern;
    const char* suggestion;
    int confidence;
} ErrorPattern;

static ErrorPattern error_patterns[] = {
    {"then", "Replace 'then' with '{' for C-style syntax", 95},
    {"begin", "Replace 'begin' with '{' for C-style syntax", 95},
    {"end", "Replace 'end' with '}' for C-style syntax", 95},
    {"procedure", "Replace 'procedure' with 'function' or 'void'", 90},
    {"function", "Check if this should be a C function definition", 80},
    {"var", "Replace 'var' with specific type like 'int', 'float', etc.", 90},
    {"const", "Use 'const' keyword correctly in C", 70},
    {"and", "Use '&&' instead of 'and' for logical AND", 95},
    {"or", "Use '||' instead of 'or' for logical OR", 95},
    {"not", "Use '!' instead of 'not' for logical NOT", 95},
    {"mod", "Use '%' instead of 'mod' for modulo", 95},
    {"div", "Use '/' instead of 'div' for division", 95},
    {NULL, NULL, 0}
};

/* AI-enhanced error detection */
int detect_potential_error(const char* lexeme, int line, int col) 
{
    if (!lexeme) return 0;
    
    /* Check against known error patterns */
    for (int i = 0; error_patterns[i].pattern != NULL; i++) 
    {
        if (strcmp(lexeme, error_patterns[i].pattern) == 0) 
        {
            return error_patterns[i].confidence;
        }
    }
    
    /* Check for common typos */
    if (strstr(lexeme, "lenght") != NULL) return 85;
    if (strstr(lexeme, "lengh") != NULL) return 85;
    if (strstr(lexeme, "widht") != NULL) return 85;
    if (strstr(lexeme, "hieght") != NULL) return 85;
    
    /* Check for mixed language keywords */
    if (strstr(lexeme, "si") != NULL && strstr(lexeme, "entonces") != NULL) return 90;
    if (strstr(lexeme, "mientras") != NULL) return 90;
    if (strstr(lexeme, "para") != NULL) return 80;
    
    /* Check for suspicious patterns */
    if (strlen(lexeme) > 50) return 70;  /* Very long identifier */
    if (strchr(lexeme, '$') != NULL) return 60;  /* Dollar sign in identifier */
    if (strchr(lexeme, '@') != NULL) return 60;  /* At sign in identifier */
    
    return 0;
}

char* suggest_fix(const char* lexeme) 
{
    if (!lexeme) return NULL;
    
    /* Check against known error patterns */
    for (int i = 0; error_patterns[i].pattern != NULL; i++) 
    {
        if (strcmp(lexeme, error_patterns[i].pattern) == 0) 
        {
            return strdup(error_patterns[i].suggestion);
        }
    }
    
    /* Generate suggestions based on patterns */
    char* suggestion = NULL;
    
    if (strstr(lexeme, "lenght") != NULL) 
    {
        suggestion = malloc(100);
        snprintf(suggestion, 100, "Did you mean 'length' instead of '%s'?", lexeme);
        return suggestion;
    }
    
    if (strstr(lexeme, "widht") != NULL) 
    {
        suggestion = malloc(100);
        snprintf(suggestion, 100, "Did you mean 'width' instead of '%s'?", lexeme);
        return suggestion;
    }
    
    if (strlen(lexeme) > 50) 
    {
        suggestion = malloc(100);
        snprintf(suggestion, 100, "Consider shortening identifier '%s' for better readability", lexeme);
        return suggestion;
    }
    
    if (strchr(lexeme, '$') != NULL || strchr(lexeme, '@') != NULL) 
    {
        suggestion = malloc(100);
        snprintf(suggestion, 100, "Remove special characters from identifier '%s'", lexeme);
        return suggestion;
    }
    
    return NULL;
}