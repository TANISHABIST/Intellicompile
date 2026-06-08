const axios = require('axios');

async function comprehensiveTest() {
  console.log('=== COMPREHENSIVE COMPILER TEST ===\n');
  
  const testCases = [
    {
      name: "1. Code with syntax error",
      code: `int main() {
    int x = 10
    return 0;
}`,
      expectedErrors: true,
      expectedOptimizations: true
    },
    {
      name: "2. Code with multiplication by 2",
      code: `int main() {
    int x = 10;
    int y = x * 2;
    return 0;
}`,
      expectedErrors: false,
      expectedOptimizations: true
    },
    {
      name: "3. Code with inefficient loop",
      code: `#include <string.h>
int main() {
    char s[] = "hello";
    for(int i = 0; i < strlen(s); i++) {
        printf("%c\\n", s[i]);
    }
    return 0;
}`,
      expectedErrors: true,
      expectedOptimizations: true
    },
    {
      name: "4. Perfect code",
      code: `#include <stdio.h>
#define MAX_SIZE 100

int main() {
    const int numbers[] = {1, 2, 3, 4, 5};
    printf("Hello World\\n");
    return 0;
}`,
      expectedErrors: true, // unused variable
      expectedOptimizations: false
    }
  ];

  for (const testCase of testCases) {
    console.log(`${testCase.name}`);
    console.log('='.repeat(50));
    
    try {
      const response = await axios.post('http://localhost:5000/api/compile', {
        code: testCase.code
      });
      
      const result = response.data;
      
      console.log(`✅ API Success`);
      console.log(`📊 Errors: ${result.errors.length} (Expected: ${testCase.expectedErrors ? 'some' : 'none'})`);
      console.log(`⚡ Optimizations: ${result.optimizations.length} (Expected: ${testCase.expectedOptimizations ? 'some' : 'none'})`);
      console.log(`📈 Quality Score: ${result.quality.overall}`);
      
      // Show errors
      if (result.errors.length > 0) {
        console.log('❌ Errors found:');
        result.errors.forEach((err, i) => {
          console.log(`   ${i+1}. Line ${err.line}: ${err.message}`);
        });
      }
      
      // Show optimizations
      if (result.optimizations.length > 0) {
        console.log('💡 Optimizations found:');
        result.optimizations.forEach((opt, i) => {
          console.log(`   ${i+1}. ${opt.title}: ${opt.description}`);
        });
      }
      
    } catch (error) {
      console.log(`❌ API Error: ${error.message}`);
      if (error.code === 'ECONNREFUSED') {
        console.log('   Backend server is not running!');
      }
    }
    
    console.log('\n');
  }
}

comprehensiveTest();
