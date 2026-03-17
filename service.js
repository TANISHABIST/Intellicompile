// Mock compiler service - replace with actual backend API calls

const mockCompilationResult = {
  errors: [
    {
      message: "Undeclared variable 'undeclared_var'",
      line: 12,
      column: 5,
      type: 'error',
      suggestion: "Declare the variable before using it"
    },
    {
      message: "Type mismatch in assignment",
      line: 13,
      column: 9,
      type: 'error',
      suggestion: "Ensure right-hand side type is compatible with left-hand side"
    },
    {
      message: "Variable 'x' declared but never used",
      line: 5,
      column: 9,
      type: 'warning',
      suggestion: "Remove unused variable or use it in your code"
    }
  ],
  optimizations: [
    {
      type: 'performance',
      title: 'Loop Optimization',
      description: 'Replace multiplication with bit shift for better performance',
      impact: 'medium',
      confidence: 85,
      before: 'result = x * 2;',
      after: 'result = x << 1;'
    },
    {
      type: 'readability',
      title: 'Variable Naming',
      description: 'Use more descriptive variable names',
      impact: 'low',
      confidence: 90,
      before: 'int x = 10;',
      after: 'int counter = 10;'
    }
  ],
  quality: {
    overall: 72,
    complexity: 65,
    readability: 78,
    maintainability: 73,
    performance: 70,
    security: 85
  }
};

export const compileCode = async (code) => {
  // Simulate API call delay
  await new Promise(resolve => setTimeout(resolve, 1500));
  
  // Mock compilation logic - in real implementation, this would call your backend
  const hasErrors = code.includes('undeclared_var') || code.includes('Type mismatch');
  const hasOptimizations = code.includes('* 2') || code.includes('int x');
  
  if (hasErrors) {
    return {
      ...mockCompilationResult,
      errors: mockCompilationResult.errors.filter(error => 
        code.includes('undeclared_var') || error.message.includes('Type mismatch')
      )
    };
  }
  
  if (hasOptimizations) {
    return {
      ...mockCompilationResult,
      errors: [],
      optimizations: mockCompilationResult.optimizations.filter(opt => 
        code.includes('* 2') || code.includes('int x')
      )
    };
  }
  
  // Perfect code
  return {
    errors: [],
    optimizations: [],
    quality: {
      overall: 92,
      complexity: 88,
      readability: 95,
      maintainability: 91,
      performance: 89,
      security: 96
    }
  };
};