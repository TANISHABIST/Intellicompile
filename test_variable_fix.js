const axios = require('axios');

async function testVariableFix() {
  const testCases = [
    {
      name: "Code with single-letter variable (should suggest)",
      code: `int func() {
    int x = 10;
    return x;
}`
    },
    {
      name: "Code with descriptive variables (should NOT suggest)",
      code: `int calculate() {
    int count = 10;
    int total = 20;
    return count + total;
}`
    },
    {
      name: "Code with main function variables (should NOT suggest)",
      code: `int main() {
    int x = 10;
    int y = 20;
    return x + y;
}`
    },
    {
      name: "Code with loop variables (should NOT suggest)",
      code: `int main() {
    for(int i = 0; i < 10; i++) {
        int x = i;
    }
    return 0;
}`
    }
  ];

  for (const testCase of testCases) {
    console.log(`\n=== ${testCase.name} ===`);
    
    try {
      const response = await axios.post('http://localhost:5000/api/compile', {
        code: testCase.code
      });
      
      const result = response.data;
      const varOptimization = result.optimizations.find(opt => opt.title === 'Variable Naming');
      
      console.log(`Variable Naming Suggestion: ${varOptimization ? 'YES' : 'NO'}`);
      console.log(`Total Optimizations: ${result.optimizations.length}`);
      
      if (result.optimizations.length > 0) {
        console.log('Optimizations:', result.optimizations.map(opt => opt.title));
      }
      
    } catch (error) {
      console.error('Error:', error.message);
    }
  }
}

testVariableFix();
