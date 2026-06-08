// Valid C program for testing
int main() {
    int x = 10;
    float y = 20.5;
    char z = 'A';
    
    if (x < y) {
        x = x + 1;
    } else {
        x = x - 1;
    }
    
    while (x > 0) {
        x = x - 1;
    }
    
    return 0;
}

int add(int a, int b) {
    return a + b;
}