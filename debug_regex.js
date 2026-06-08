// Test regex patterns
const testCases = [
  {
    name: "Main function code",
    code: `int main() {
    int x = 10;
    int y = 20;
    return x + y;
}`
  },
  {
    name: "Regular function code",
    code: `int calculate() {
    int x = 10;
    return x;
}`
  },
  {
    name: "Loop code",
    code: `int main() {
    for(int i = 0; i < 10; i++) {
        int x = i;
    }
    return 0;
}`
  }
];

const patterns = {
  hasMainFunction: /\bint\s+main\s*\([^)]*\)\s*\{/,
  hasLoops: /\b(for|while|do)\s*\([^)]*\)/,
  singleVarPattern: /\b(int|float|char|double)\s+([b-hj-z])\s*=\s*\d+/
};

testCases.forEach(testCase => {
  console.log(`\n=== ${testCase.name} ===`);
  
  Object.keys(patterns).forEach(patternName => {
    const matches = patterns[patternName].test(testCase.code);
    console.log(`${patternName}: ${matches ? 'MATCH' : 'NO MATCH'}`);
  });
  
  const shouldSuggest = (patterns.singleVarPattern.test(testCase.code) || 
                      /\b(int|float|char|double)\s+([b-hj-z])\s*=\s*"[^"]*"/.test(testCase.code) || 
                      /\b(int|float|char|double)\s+([b-hj-z])\s*=\s*[^a-zA-Z]/.test(testCase.code)) && 
                      !patterns.hasMainFunction.test(testCase.code) && 
                      !patterns.hasLoops.test(testCase.code);
  
  console.log(`Should suggest Variable Naming: ${shouldSuggest ? 'YES' : 'NO'}`);
});
