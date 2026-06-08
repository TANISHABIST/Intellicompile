const { spawn } = require('child_process');
const path = require('path');

console.log('🚀 Starting IntelliCompile Services...\n');

// Start backend server
console.log('📡 Starting backend server on port 5000...');
const backend = spawn('node', ['server.js'], {
  cwd: __dirname,
  stdio: 'pipe'
});

backend.stdout.on('data', (data) => {
  console.log(`[Backend] ${data.toString().trim()}`);
});

backend.stderr.on('data', (data) => {
  console.log(`[Backend Error] ${data.toString().trim()}`);
});

backend.on('close', (code) => {
  console.log(`Backend process exited with code ${code}`);
});

// Wait a moment for backend to start, then start frontend
setTimeout(() => {
  console.log('\n🌐 Starting React frontend...');
  const frontend = spawn('npm', ['start'], {
    cwd: __dirname,
    stdio: 'pipe'
  });

  frontend.stdout.on('data', (data) => {
    console.log(`[Frontend] ${data.toString().trim()}`);
  });

  frontend.stderr.on('data', (data) => {
    console.log(`[Frontend Error] ${data.toString().trim()}`);
  });

  frontend.on('close', (code) => {
    console.log(`Frontend process exited with code ${code}`);
  });

  console.log('\n✅ Both services started!');
  console.log('📡 Backend: http://localhost:5000');
  console.log('🌐 Frontend: http://localhost:3000');
  console.log('\n💡 Open http://localhost:3000 in your browser to use the application');
}, 2000);

// Handle shutdown
process.on('SIGINT', () => {
  console.log('\n🛑 Shutting down services...');
  backend.kill();
  process.exit(0);
});
