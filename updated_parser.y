/* Add to the C declarations section */
#include "semantic_analyzer.h"

/* Global semantic context */
SemanticContext* semantic_context = NULL;

/* Add to main function */
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
        printf("Syntax analysis completed successfully.\n");
        
        /* Perform semantic analysis */
        printf("\nStarting semantic analysis...\n");
        semantic_context = create_semantic_context();
        
        int semantic_result = analyze_ast(program_ast, semantic_context);
        
        print_semantic_errors(semantic_context->errors);
        
        if (semantic_result) {
            printf("\nSemantic analysis completed successfully.\n");
        } else {
            printf("\nSemantic analysis failed.\n");
        }
        
        free_semantic_context(semantic_context);
    } else {
        printf("\nSyntax analysis failed with %d errors.\n", syntax_errors);
    }
    
    if (program_ast) {
        free_ast(program_ast);
    }
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return (syntax_errors > 0 || (semantic_context && semantic_context->error_count > 0)) ? 1 : 0;
}