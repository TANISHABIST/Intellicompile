#ifndef SYNTAX_ERROR_DETECTOR_H
#define SYNTAX_ERROR_DETECTOR_H

int detect_syntax_error_pattern(int token_type, const char* lexeme);
char* suggest_syntax_fix(int token_type, const char* lexeme);

#endif