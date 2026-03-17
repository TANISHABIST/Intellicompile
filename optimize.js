import React from 'react';
import { Lightbulb, Zap, Code, CheckCircle } from 'lucide-react';

const OptimizationPanel = ({ optimizations }) => {
  if (!optimizations || optimizations.length === 0) {
    return (
      <div className="flex flex-col items-center justify-center h-full text-gray-500">
        <Lightbulb className="w-12 h-12 mb-4 text-gray-400" />
        <p className="text-lg font-medium">No optimizations available</p>
        <p className="text-sm">Your code is already optimized!</p>
      </div>
    );
  }

  const getOptimizationIcon = (type) => {
    switch (type) {
      case 'performance':
        return <Zap className="w-5 h-5 text-yellow-500" />;
      case 'memory':
        return <Code className="w-5 h-5 text-blue-500" />;
      case 'readability':
        return <Lightbulb className="w-5 h-5 text-green-500" />;
      default:
        return <CheckCircle className="w-5 h-5 text-purple-500" />;
    }
  };

  const getOptimizationColor = (type) => {
    switch (type) {
      case 'performance':
        return 'border-yellow-500 bg-yellow-50';
      case 'memory':
        return 'border-blue-500 bg-blue-50';
      case 'readability':
        return 'border-green-500 bg-green-50';
      default:
        return 'border-purple-500 bg-purple-50';
    }
  };

  const getImpactColor = (impact) => {
    switch (impact) {
      case 'high':
        return 'text-red-600 bg-red-100';
      case 'medium':
        return 'text-yellow-600 bg-yellow-100';
      case 'low':
        return 'text-green-600 bg-green-100';
      default:
        return 'text-gray-600 bg-gray-100';
    }
  };

  return (
    <div className="p-4 space-y-3">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-gray-900">AI Optimization Suggestions</h3>
        <span className="text-sm text-gray-500">{optimizations.length} suggestions</span>
      </div>

      {optimizations.map((opt, index) => (
        <div
          key={index}
          className={`p-4 rounded-lg border-l-4 ${getOptimizationColor(opt.type)} fade-in`}
        >
          <div className="flex items-start space-x-3">
            {getOptimizationIcon(opt.type)}
            
            <div className="flex-1">
              <div className="flex items-center justify-between mb-2">
                <h4 className="text-sm font-semibold text-gray-900 capitalize">
                  {opt.title}
                </h4>
                <span className={`text-xs px-2 py-1 rounded-full font-medium ${getImpactColor(opt.impact)}`}>
                  {opt.impact} impact
                </span>
              </div>
              
              <p className="text-sm text-gray-700 mb-3">{opt.description}</p>
              
              {opt.before && opt.after && (
                <div className="space-y-2">
                  <div className="bg-gray-900 text-gray-100 p-3 rounded font-mono text-xs">
                    <div className="text-red-400 mb-1">// Before:</div>
                    <pre>{opt.before}</pre>
                  </div>
                  <div className="bg-gray-900 text-gray-100 p-3 rounded font-mono text-xs">
                    <div className="text-green-400 mb-1">// After:</div>
                    <pre>{opt.after}</pre>
                  </div>
                </div>
              )}
              
              <div className="mt-3 flex items-center justify-between">
                <div className="flex items-center space-x-2">
                  <span className="text-xs text-gray-500">Confidence:</span>
                  <div className="flex items-center space-x-1">
                    {[...Array(5)].map((_, i) => (
                      <div
                        key={i}
                        className={`w-2 h-2 rounded-full ${
                          i < Math.floor(opt.confidence / 20)
                            ? 'bg-green-500'
                            : 'bg-gray-300'
                        }`}
                      />
                    ))}
                    <span className="text-xs text-gray-600 ml-1">{opt.confidence}%</span>
                  </div>
                </div>
                
                <button className="text-xs bg-indigo-600 text-white px-3 py-1 rounded hover:bg-indigo-700 transition-colors">
                  Apply Fix
                </button>
              </div>
            </div>
          </div>
        </div>
      ))}
    </div>
  );
};

export default OptimizationPanel;