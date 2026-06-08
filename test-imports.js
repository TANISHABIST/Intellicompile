console.log('Test starting...');

try {
  console.log('Importing React...');
  const React = require('react');
  console.log('✓ React imported');
  
  console.log('Importing components...');
  const app = require('./src/app.js');
  console.log('✓ App imported successfully');
  
  console.log('All imports successful!');
} catch (error) {
  console.error('ERROR:', error.message);
  console.error('Full error:', error);
  process.exit(1);
}
