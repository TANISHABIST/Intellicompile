// Test custom error detection patterns
const testCode = `int main() {
    int num = 10;
    char ch = 'A';
    int result = num + ch;
    return 0;
}`;

const lines = testCode.split('\n');

console.log('Testing custom error detection patterns:');
console.log('Code:', testCode);
console.log('='.repeat(50));

lines.forEach((line, index) => {
  const lineNum = index + 1;
  const trimmedLine = line.trim();
  
  console.log(`\nLine ${lineNum}: "${trimmedLine}"`);
  
  // Test int + char patterns
  const pattern1 = trimmedLine.match(/\b(int|float|double)\s+\w+\s*\+\s*char\s+\w+/);
  const pattern2 = trimmedLine.match(/\bchar\s+\w+\s*\+\s*(int|float|double)\s+\w+/);
  const pattern3 = trimmedLine.match(/\w+\s*\+\s*'[A-Za-z]'/);
  const pattern4 = trimmedLine.match(/'[A-Za-z]'\s*\+\s*\w+/);
  
  console.log(`  Pattern 1 (int + char): ${pattern1 ? 'MATCH' : 'NO MATCH'}`);
  console.log(`  Pattern 2 (char + int): ${pattern2 ? 'MATCH' : 'NO MATCH'}`);
  console.log(`  Pattern 3 (var + 'char'): ${pattern3 ? 'MATCH' : 'NO MATCH'}`);
  console.log(`  Pattern 4 ('char' + var): ${pattern4 ? 'MATCH' : 'NO MATCH'}`);
  
  const shouldDetect = pattern1 || pattern2 || pattern3 || pattern4;
  console.log(`  Should detect error: ${shouldDetect ? 'YES' : 'NO'}`);
});
