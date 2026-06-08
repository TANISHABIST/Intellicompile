const axios = require('axios');

async function testIntCharAddition() {
  const testCode = `int main() {
    int num = 10;
    char ch = 'A';
    int result = num + ch;
    return 0;
}`;

  console.log('Testing int + char addition:');
  console.log('Code:', testCode);
  console.log('='.repeat(50));
  
  try {
    const response = await axios.post('http://localhost:5000/api/compile', {
      code: testCode
    });
    
    const result = response.data;
    
    console.log(`✅ Compilation Status: ${result.compiled ? 'SUCCESS' : 'FAILED'}`);
    console.log(`📊 Errors Found: ${result.errors.length}`);
    console.log(`⚡ Optimizations: ${result.optimizations.length}`);
    console.log(`📈 Quality Score: ${result.quality.overall}`);
    
    if (result.errors.length > 0) {
      console.log('\n❌ Errors:');
      result.errors.forEach((err, i) => {
        console.log(`   ${i+1}. Line ${err.line}:${err.column} - ${err.message} (${err.type})`);
        console.log(`      💡 Suggestion: ${err.suggestion}`);
      });
    }
    
    if (result.optimizations.length > 0) {
      console.log('\n💡 Optimizations:');
      result.optimizations.forEach((opt, i) => {
        console.log(`   ${i+1}. ${opt.title}: ${opt.description}`);
      });
    }
    
  } catch (error) {
    console.error('❌ API Error:', error.message);
  }
}

testIntCharAddition();
