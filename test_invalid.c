// Invalid C program with missing semicolons for testing
int main() {
    int x = 10          // Missing semicolon
    float y = 20.5      // Missing semicolon
    char z = 'A'        // Missing semicolon
    
    if (x < y) {
        x = x + 1       // Missing semicolon
    } else {
        x = x - 1       // Missing semicolon
    }
    
    while (x > 0) {
        x = x - 1       // Missing semicolon
    }
    
    return 0            // Missing semicolon
}

int add(int a, int b) {
    return a + b        // Missing semicolon
}