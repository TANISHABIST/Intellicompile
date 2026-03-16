// Sample C program for testing the lexer
int main() {
    int x = 10;
    float y = 20.5;
    char z = 'A';
    
    // Test various tokens
    if (x < y) 
    {
        x = x + 1;
    } else 
    {
        x = x - 1;
    }
    
    // Test operators
    x = x * 2;
    y = y / 3.0;
    z = z + 1;
    
    // Test invalid tokens (should trigger AI error detection)
    // then begin end var procedure
    
    return 0;
}