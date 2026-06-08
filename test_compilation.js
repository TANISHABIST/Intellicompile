const http = require('http');

// Test data
const testCases = [
  {
    name: "Simple code with multiplication",
    code: `int main() {
    int x = 10;
    x = x * 2;
    return 0;
}`
  },
  {
    name: "Code with inefficient loop",
    code: `#include <string.h>
int main() {
    char s[] = "hello";
    for(int i = 0; i < strlen(s); i++) {
        printf("%c\\n", s[i]);
    }
    return 0;
}`
  },
  {
    name: "Code with syntax error",
    code: `int main() {
    int x = 10
    return 0;
}`
  },
  {
    name: "Code with good practices",
    code: `#include <stdio.h>
#define MAX_SIZE 100

int calculate_sum(const int* arr, int size) {
    int total = 0;
    for(int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

int main() {
    const int numbers[] = {1, 2, 3, 4, 5};
    int result = calculate_sum(numbers, 5);
    printf("Sum: %d\\n", result);
    return 0;
}`
  }
];

// Test each case
testCases.forEach((testCase, index) => {
  console.log(`\n=== Test Case ${index + 1}: ${testCase.name} ===`);
  
  const postData = JSON.stringify({ code: testCase.code });
  
  const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/api/compile',
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'Content-Length': Buffer.byteLength(postData)
    }
  };

  const req = http.request(options, (res) => {
    let data = '';
    
    res.on('data', (chunk) => {
      data += chunk;
    });
    
    res.on('end', () => {
      try {
        const result = JSON.parse(data);
        console.log(`Errors: ${result.errors.length}`);
        console.log(`Optimizations: ${result.optimizations.length}`);
        console.log(`Quality Score: ${result.quality.overall}`);
        
        if (result.errors.length > 0) {
          console.log('First error:', result.errors[0].message);
        }
        
        if (result.optimizations.length > 0) {
          console.log('First optimization:', result.optimizations[0].title);
        }
        
        console.log('Quality metrics:', result.quality);
      } catch (e) {
        console.log('Error parsing response:', e.message);
        console.log('Raw response:', data);
      }
    });
  });

  req.on('error', (e) => {
    console.log(`Request error: ${e.message}`);
  });

  req.write(postData);
  req.end();
});
