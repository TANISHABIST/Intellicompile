// Valid program for semantic testing
int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 10;
    int y = 20;
    int result;
    
    result = add(x, y);
    
    if (result > 15) {
        x = x + 1;
    }
    
    while (x < 100) {
        x = x * 2;
    }
    
    return 0;
}