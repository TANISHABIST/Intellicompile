const axios = require('axios');

async function testAPI() {
  const testCases = [
    {
      name: "Code with syntax error",
      code: `int main() {
    int x = 10
    return 0;
}`
    },
    {
      name: "Code with multiplication by 2",
      code: `int main() {
    int x = 10;
    int y = x * 2;
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
      name: "Perfect code",
      code: `#include <stdio.h>
#define MAX_SIZE 100

int main() {
    const int numbers[] = {1, 2, 3, 4, 5};
    printf("Hello World\\n");
    return 0;
}`
    }
  ];

  for (let i = 0; i < testCases.length; i++) {
    const testCase = testCases[i];
    console.log(`\n=== Test ${i + 1}: ${testCase.name} ===`);
    
    try {
      const response = await axios.post('http://localhost:5000/api/compile', {
        code: testCase.code
      });
      
      const result = response.data;
      console.log(`Errors: ${result.errors.length}`);
      console.log(`Optimizations: ${result.optimizations.length}`);
      console.log(`Quality Score: ${result.quality.overall}`);
      
      if (result.errors.length > 0) {
        console.log('First error:', result.errors[0].message);
      }
      
      if (result.optimizations.length > 0) {
        console.log('First optimization:', result.optimizations[0].title);
      }
      
      console.log('All optimizations:', result.optimizations.map(opt => opt.title));
      
    } catch (error) {
      console.log('API Error:', error.message);
    }
  }
}

testAPI();
