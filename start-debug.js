process.env.BROWSER = 'none';
process.env.SKIP_PREFLIGHT_CHECK = 'true';

try {
  require('react-scripts/scripts/start.js');
} catch (error) {
  console.error('ERROR:', error);
  console.error('Stack:', error.stack);
  process.exit(1);
}
