import React from 'react';
import { BarChart3, TrendingUp, Code, Target } from 'lucide-react';
import { Line, Bar } from 'react-chartjs-2';
import 
{
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  BarElement,
  Title,
  Tooltip,
  Legend,
  RadialLinearScale,
  ArcElement,
} from 'chart.js';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  BarElement,
  Title,
  Tooltip,
  Legend,
  RadialLinearScale,
  ArcElement
);

const QualityDashboard = ({ quality }) => {
  const getScoreColor = (score) => {
    if (score >= 80) return 'text-green-600';
    if (score >= 60) return 'text-blue-600';
    if (score >= 40) return 'text-yellow-600';
    return 'text-red-600';
  };

  const getScoreLabel = (score) => {
    if (score >= 80) return 'Excellent';
    if (score >= 60) return 'Good';
    if (score >= 40) return 'Fair';
    return 'Poor';
  };

  const overallScore = quality.overall || 0;
  const complexity = quality.complexity || 0;
  const readability = quality.readability || 0;
  const maintainability = quality.maintainability || 0;

  // Radar chart data
  const radarData = {
    labels: ['Complexity', 'Readability', 'Maintainability', 'Performance', 'Security'],
    datasets: [
      {
        label: 'Code Quality',
        data: [complexity, readability, maintainability, quality.performance || 70, quality.security || 80],
        backgroundColor: 'rgba(99, 102, 241, 0.2)',
        borderColor: 'rgba(99, 102, 241, 1)',
        borderWidth: 2,
        pointBackgroundColor: 'rgba(99, 102, 241, 1)',
        pointBorderColor: '#fff',
        pointHoverBackgroundColor: '#fff',
        pointHoverBorderColor: 'rgba(99, 102, 241, 1)',
      },
    ],
  };

  // Bar chart data
  const barData = {
    labels: ['Complexity', 'Readability', 'Maintainability'],
    datasets: [
      {
        label: 'Quality Score',
        data: [complexity, readability, maintainability],
        backgroundColor: [
          'rgba(239, 68, 68, 0.8)',
          'rgba(59, 130, 246, 0.8)',
          'rgba(16, 185, 129, 0.8)',
        ],
        borderColor: [
          'rgba(239, 68, 68, 1)',
          'rgba(59, 130, 246, 1)',
          'rgba(16, 185, 129, 1)',
        ],
        borderWidth: 1,
      },
    ],
  };

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        display: false,
      },
    },
    scales: {
      r: {
        beginAtZero: true,
        max: 100,
        ticks: {
          stepSize: 20,
        },
      },
    },
  };

  const barOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        display: false,
      },
    },
    scales: {
      y: {
        beginAtZero: true,
        max: 100,
      },
    },
  };

  return (
    <div className="p-4 space-y-6">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-gray-900">Code Quality Analysis</h3>
        <div className="flex items-center space-x-2">
          <BarChart3 className="w-5 h-5 text-indigo-600" />
          <span className="text-sm text-gray-500">AI-Powered Analysis</span>
        </div>
      </div>

      {/* Overall Score */}
      <div className="bg-gradient-to-r from-indigo-500 to-purple-600 rounded-xl p-6 text-white">
        <div className="flex items-center justify-between">
          <div>
            <h4 className="text-2xl font-bold mb-2">Overall Quality Score</h4>
            <div className="flex items-center space-x-3">
              <span className={`text-4xl font-bold ${getScoreColor(overallScore)}`}>
                {overallScore}
              </span>
              <div>
                <span className="text-lg font-medium">{getScoreLabel(overallScore)}</span>
                <p className="text-sm opacity-90">Out of 100</p>
              </div>
            </div>
          </div>
          <div className="text-right">
            <TrendingUp className="w-8 h-8 mb-2" />
            <p className="text-sm">AI Assessed</p>
          </div>
        </div>
      </div>

      {/* Quality Metrics */}
      <div className="grid grid-cols-3 gap-4">
        <div className="bg-white rounded-lg p-4 border border-gray-200">
          <div className="flex items-center justify-between mb-2">
            <Code className="w-5 h-5 text-red-500" />
            <span className={`text-lg font-bold ${getScoreColor(complexity)}`}>
              {complexity}
            </span>
          </div>
          <p className="text-sm text-gray-600">Complexity</p>
          <div className="w-full bg-gray-200 rounded-full h-2 mt-2">
            <div
              className="bg-red-500 h-2 rounded-full transition-all duration-500"
              style={{ width: `${complexity}%` }}
            />
          </div>
        </div>

        <div className="bg-white rounded-lg p-4 border border-gray-200">
          <div className="flex items-center justify-between mb-2">
            <Target className="w-5 h-5 text-blue-500" />
            <span className={`text-lg font-bold ${getScoreColor(readability)}`}>
              {readability}
            </span>
          </div>
          <p className="text-sm text-gray-600">Readability</p>
          <div className="w-full bg-gray-200 rounded-full h-2 mt-2">
            <div
              className="bg-blue-500 h-2 rounded-full transition-all duration-500"
              style={{ width: `${readability}%` }}
            />
          </div>
        </div>

        <div className="bg-white rounded-lg p-4 border border-gray-200">
          <div className="flex items-center justify-between mb-2">
            <BarChart3 className="w-5 h-5 text-green-500" />
            <span className={`text-lg font-bold ${getScoreColor(maintainability)}`}>
              {maintainability}
            </span>
          </div>
          <p className="text-sm text-gray-600">Maintainability</p>
          <div className="w-full bg-gray-200 rounded-full h-2 mt-2">
            <div
              className="bg-green-500 h-2 rounded-full transition-all duration-500"
              style={{ width: `${maintainability}%` }}
            />
          </div>
        </div>
      </div>

      {/* Charts */}
      <div className="grid grid-cols-2 gap-4">
        <div className="bg-white rounded-lg p-4 border border-gray-200">
          <h4 className="text-sm font-semibold text-gray-900 mb-4">Quality Radar</h4>
          <div className="h-48">
            <Line data={radarData} options={chartOptions} />
          </div>
        </div>

        <div className="bg-white rounded-lg p-4 border border-gray-200">
          <h4 className="text-sm font-semibold text-gray-900 mb-4">Score Breakdown</h4>
          <div className="h-48">
            <Bar data={barData} options={barOptions} />
          </div>
        </div>
      </div>

      {/* Recommendations */}
      <div className="bg-gray-50 rounded-lg p-4">
        <h4 className="text-sm font-semibold text-gray-900 mb-3">AI Recommendations</h4>
        <div className="space-y-2">
          {complexity < 60 && (
            <div className="flex items-start space-x-2">
              <div className="w-2 h-2 bg-red-500 rounded-full mt-1.5" />
              <p className="text-sm text-gray-700">
                Consider reducing code complexity by breaking down large functions
              </p>
            </div>
          )}
          {readability < 60 && (
            <div className="flex items-start space-x-2">
              <div className="w-2 h-2 bg-blue-500 rounded-full mt-1.5" />
              <p className="text-sm text-gray-700">
                Improve readability with better variable names and comments
              </p>
            </div>
          )}
          {maintainability < 60 && (
            <div className="flex items-start space-x-2">
              <div className="w-2 h-2 bg-green-500 rounded-full mt-1.5" />
              <p className="text-sm text-gray-700">
                Enhance maintainability by following coding standards
              </p>
            </div>
          )}
          {overallScore >= 80 && (
            <div className="flex items-start space-x-2">
              <div className="w-2 h-2 bg-green-500 rounded-full mt-1.5" />
              <p className="text-sm text-gray-700">
                Excellent code quality! Keep up the good work!
              </p>
            </div>
          )}
        </div>
      </div>
    </div>
  );
};

export default QualityDashboard;