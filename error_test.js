const axios = require('axios');

async function errorTest() {
  const testCode = `int main() {
    int x = 10
    return 0;
}`;

  console.log('Testing with error code:', testCode);
  
  try {
    const response = await axios.post('http://localhost:5000/api/compile', {
      code: testCode
    });
    
    console.log('Full response:', JSON.stringify(response.data, null, 2));
    
    console.log('\n--- Analysis ---');
    console.log('Errors:', response.data.errors.length);
    response.data.errors.forEach((err, i) => {
      console.log(`  ${i+1}. Line ${err.line}:${err.column} - ${err.message} (${err.type})`);
      console.log(`     Suggestion: ${err.suggestion}`);
    });
    
    console.log('Optimizations:', response.data.optimizations.length);
    response.data.optimizations.forEach((opt, i) => {
      console.log(`  ${i+1}. ${opt.title} - ${opt.description}`);
    });
    
  } catch (error) {
    console.error('Error:', error.message);
  }
}

errorTest();
