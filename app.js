import React, { useState, useCallback } from 'react';
import { Code, Play, AlertCircle, Lightbulb, BarChart3, Settings, Github } from 'lucide-react';
import CodeEditor from './components/CodeEditor';
import ErrorPanel from './components/ErrorPanel';
import OptimizationPanel from './components/OptimizationPanel';
import QualityDashboard from './components/QualityDashboard';
import Header from './components/Header';
import { compileCode } from './services/compilerService';

function App() {
  const [code, setCode] = useState(`// Welcome to IntelliCompile - AI-Enabled Smart Compiler
// Enter your C code below and click Compile to see AI-powered analysis

int main() {
    int x = 10;
    int y = 20;
    int result;
    
    result = x + y;
    
    if (result > 15) {
        printf("Result is greater than 15\\n");
    }
    
    return 0;
}`);

  const [compilationResult, setCompilationResult] = useState(null);
  const [isCompiling, setIsCompiling] = useState(false);
  const [activePanel, setActivePanel] = useState('errors');

  const handleCompile = useCallback(async () => {
    setIsCompiling(true);
    
    try {
      const result = await compileCode(code);
      setCompilationResult(result);
      
      // Auto-switch to the most relevant panel
      if (result.errors && result.errors.length > 0) {
        setActivePanel('errors');
      } else if (result.optimizations && result.optimizations.length > 0) {
        setActivePanel('optimizations');
      } else {
        setActivePanel('quality');
      }
    } catch (error) {
      console.error('Compilation failed:', error);
      setCompilationResult({
        errors: [{ message: 'Compilation service unavailable', line: 1, column: 1, type: 'error' }],
        optimizations: [],
        quality: { overall: 0, complexity: 0, readability: 0, maintainability: 0 }
      });
    } finally {
      setIsCompiling(false);
    }
  }, [code]);

  const handleCodeChange = useCallback((newCode) => {
    setCode(newCode);
    setCompilationResult(null);
  }, []);

  return (
    <div className="min-h-screen bg-gradient-to-br from-indigo-50 to-purple-50">
      <Header />
      
      <main className="container mx-auto px-4 py-6">
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          
          {/* Left Panel - Code Editor */}
          <div className="space-y-4">
            <div className="bg-white rounded-xl shadow-lg overflow-hidden">
              <div className="bg-gradient-to-r from-indigo-600 to-purple-600 text-white p-4">
                <div className="flex items-center justify-between">
                  <div className="flex items-center space-x-2">
                    <Code className="w-5 h-5" />
                    <h2 className="text-lg font-semibold">Code Editor</h2>
                  </div>
                  <button
                    onClick={handleCompile}
                    disabled={isCompiling}
                    className="flex items-center space-x-2 bg-white text-indigo-600 px-4 py-2 rounded-lg font-medium hover:bg-indigo-50 transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
                  >
                    <Play className="w-4 h-4" />
                    {isCompiling ? 'Compiling...' : 'Compile'}
                  </button>
                </div>
              </div>
              
              <div className="h-96 lg:h-[500px]">
                <CodeEditor
                  value={code}
                  onChange={handleCodeChange}
                  errors={compilationResult?.errors || []}
                />
              </div>
            </div>
          </div>

          {/* Right Panel - Results */}
          <div className="space-y-4">
            {/* Panel Tabs */}
            <div className="bg-white rounded-xl shadow-lg overflow-hidden">
              <div className="flex border-b">
                <button
                  onClick={() => setActivePanel('errors')}
                  className={`flex-1 flex items-center justify-center space-x-2 py-3 px-4 font-medium transition-colors ${
                    activePanel === 'errors'
                      ? 'bg-red-50 text-red-600 border-b-2 border-red-600'
                      : 'text-gray-600 hover:bg-gray-50'
                  }`}
                >
                  <AlertCircle className="w-4 h-4" />
                  <span>Errors</span>
                  {compilationResult?.errors?.length > 0 && (
                    <span className="bg-red-100 text-red-600 px-2 py-1 rounded-full text-xs">
                      {compilationResult.errors.length}
                    </span>
                  )}
                </button>
                
                <button
                  onClick={() => setActivePanel('optimizations')}
                  className={`flex-1 flex items-center justify-center space-x-2 py-3 px-4 font-medium transition-colors ${
                    activePanel === 'optimizations'
                      ? 'bg-green-50 text-green-600 border-b-2 border-green-600'
                      : 'text-gray-600 hover:bg-gray-50'
                  }`}
                >
                  <Lightbulb className="w-4 h-4" />
                  <span>Optimizations</span>
                  {compilationResult?.optimizations?.length > 0 && (
                    <span className="bg-green-100 text-green-600 px-2 py-1 rounded-full text-xs">
                      {compilationResult.optimizations.length}
                    </span>
                  )}
                </button>
                
                <button
                  onClick={() => setActivePanel('quality')}
                  className={`flex-1 flex items-center justify-center space-x-2 py-3 px-4 font-medium transition-colors ${
                    activePanel === 'quality'
                      ? 'bg-blue-50 text-blue-600 border-b-2 border-blue-600'
                      : 'text-gray-600 hover:bg-gray-50'
                  }`}
                >
                  <BarChart3 className="w-4 h-4" />
                  <span>Quality</span>
                </button>
              </div>

              {/* Panel Content */}
              <div className="h-96 lg:h-[500px] overflow-y-auto">
                {activePanel === 'errors' && (
                  <ErrorPanel errors={compilationResult?.errors || []} />
                )}
                {activePanel === 'optimizations' && (
                  <OptimizationPanel optimizations={compilationResult?.optimizations || []} />
                )}
                {activePanel === 'quality' && (
                  <QualityDashboard quality={compilationResult?.quality || {}} />
                )}
              </div>
            </div>
          </div>
        </div>
      </main>
    </div>
  );
}

export default App;