// Invalid program for semantic testing
int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 10;
    float y = 20.5;
    int result;
    
    // Semantic errors:
    undeclared_var = 5;        // Undeclared variable
    x = "hello";              // Type mismatch
    result = add(x, "test");  // Wrong parameter types
    result = add(x);          // Wrong number of parameters
    unknown_func();           // Undeclared function
    
    if (x > y) {              // Type mismatch in comparison
        x = x + 1;
    }
    
    break;                    // Break outside loop
    
    return "wrong";            // Wrong return type
}

int duplicate_var() {
    int x;
    int x;                    // Duplicate declaration
    return 0;
}