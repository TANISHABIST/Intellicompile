const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');
const cors = require('cors');
const bodyParser = require('body-parser');

const app = express();

const PORT = 5000;

/* =========================
   MIDDLEWARE
========================= */

app.use(cors());

app.use(bodyParser.json());

app.use(express.json({
  limit: '50mb'
}));

/* =========================
   HEALTH CHECK
========================= */

app.get('/api/health', (req, res) => {

  res.json({
    status: 'ok',
    message: 'Compiler service is running'
  });

});

/* =========================
   EXECUTE USER CODE
========================= */

app.post('/execute', (req, res) => {

  try {

    const code = req.body.code;

    if (!code) {

      return res.status(400).json({

        success: false,

        output: 'No code provided'
      });
    }

    // TEMP FILES

    const tempFile =
      path.join(
        __dirname,
        `temp_${Date.now()}.c`
      );

    const outputFile =
      tempFile.replace('.c', '');

    // SAVE CODE

    fs.writeFileSync(
      tempFile,
      code
    );

    // COMPILE + RUN

    const command =
      `gcc "${tempFile}" -o "${outputFile}" && "${outputFile}"`;

    exec(

      command,

      {
        timeout: 10000
      },

      (error, stdout, stderr) => {

        // CLEANUP

        try {

          if (
            fs.existsSync(tempFile)
          ) {

            fs.unlinkSync(tempFile);
          }

          if (
            fs.existsSync(outputFile)
          ) {

            fs.unlinkSync(outputFile);
          }

        } catch (cleanupError) {

          console.log(
            'Cleanup error:',
            cleanupError.message
          );
        }

        // COMPILATION / RUNTIME ERROR

        if (error) {

          return res.json({

            success: false,

            output:
              stderr ||
              error.message
          });
        }

        // SUCCESS OUTPUT

        return res.json({

          success: true,

          output:
            stdout ||
            'Program executed successfully'
        });
      }

    );

  } catch (error) {

    return res.status(500).json({

      success: false,

      output:
        error.message
    });
  }
});

/* =========================
   HELPER FUNCTION
========================= */

function executeCommand(
  command,
  timeout = 10000
) {

  return new Promise((resolve) => {

    exec(

      command,

      {
        timeout,
        cwd: __dirname
      },

      (
        error,
        stdout,
        stderr
      ) => {

        resolve({
          error,
          stdout,
          stderr
        });
      }
    );
  });
}

/* =========================
   ERROR PARSER
========================= */

function parseErrors(output) {

  const errors = [];

  const lines =
    output.split('\n');

  lines.forEach((line) => {

    const match =
      line.match(
        /.*:(\d+):(\d+):\s*(error|warning):\s*(.+)/
      );

    if (match) {

      const errorMessage =
        match[4].trim();

      const errorType =
        match[3].toLowerCase();

      let suggestion = '';

      if (errorType === 'error') {

        if (
          errorMessage.includes(
            'undeclared'
          )
        ) {

          suggestion =
            'Check if the variable or function is declared before use';

        } else if (
          errorMessage.includes(
            'syntax error'
          )
        ) {

          suggestion =
            'Check syntax, brackets, and semicolons';

        } else if (
          errorMessage.includes(
            'type'
          )
        ) {

          suggestion =
            'Check variable types and conversions';

        } else {

          suggestion =
            'Review the code near this line';
        }

      } else {

        if (
          errorMessage.includes(
            'unused'
          )
        ) {

          suggestion =
            'Remove unused variables';

        } else {

          suggestion =
            'Consider fixing this warning';
        }
      }

      errors.push({

        line:
          parseInt(match[1]),

        column:
          parseInt(match[2]),

        type:
          errorType,

        message:
          errorMessage,

        suggestion
      });
    }
  });

  return errors;
}

/* =========================
   OPTIMIZATION ANALYZER
========================= */

function analyzeOptimizations(code) {

  const optimizations = [];

  // MULTIPLICATION BY 2

  if (
    /\w+\s*\*\s*2/.test(code)
  ) {

    optimizations.push({

      type: 'performance',

      title:
        'Bit Shift Optimization',

      description:
        'Use bit shift instead of multiplication by 2',

      impact: 'low',

      confidence: 85,

      before: 'x * 2',

      after: 'x << 1'
    });
  }

  // MAGIC NUMBERS

  const magicNumbers =
    code.match(
      /\b(256|512|1024|2048)\b/g
    );

  if (magicNumbers) {

    optimizations.push({

      type:
        'maintainability',

      title:
        'Magic Numbers',

      description:
        'Replace magic numbers with named constants',

      impact: 'medium',

      confidence: 90,

      before:
        'if(size > 256)',

      after:
        '#define MAX_SIZE 256'
    });
  }

  return optimizations;
}

/* =========================
   DUPLICATE LINE CHECK
========================= */

function findDuplicateLines(code) {

  const lines =
    code
      .split('\n')
      .map(line => line.trim())
      .filter(line => line.length > 5);

  const counts = {};

  let duplicates = 0;

  lines.forEach((line) => {

    counts[line] =
      (counts[line] || 0) + 1;

    if (counts[line] > 1) {

      duplicates++;
    }
  });

  return duplicates;
}

/* =========================
   QUALITY METRICS
========================= */

function calculateQualityMetrics(
  code,
  errors
) {

  const errorCount =
    errors.filter(
      e => e.type === 'error'
    ).length;

  const warningCount =
    errors.filter(
      e => e.type === 'warning'
    ).length;

  // COMPLEXITY

  const ifs =
    (code.match(/\bif\b/g) || []).length;

  const loops =
    (code.match(/\b(for|while|do)\b/g) || []).length;

  const switches =
    (code.match(/\bswitch\b/g) || []).length;

  let complexity =
    100 -
    (
      ifs * 5 +
      loops * 5 +
      switches * 4
    );

  complexity =
    Math.max(20, complexity);

  // READABILITY

  let readability = 80;

  if (
    /\/\/|\/\*/.test(code)
  ) {

    readability += 10;
  }

  const longLines =
    code
      .split('\n')
      .filter(
        line => line.length > 100
      ).length;

  readability -=
    longLines * 2;

  readability =
    Math.max(
      0,
      Math.min(100, readability)
    );

  // MAINTAINABILITY

  let maintainability = 90;

  maintainability -=
    warningCount * 5;

  maintainability -=
    errorCount * 15;

  maintainability -=
    findDuplicateLines(code);

  maintainability =
    Math.max(
      0,
      Math.min(
        100,
        maintainability
      )
    );

  // PERFORMANCE

  let performance = 85;

  if (
    code.includes('strlen(') &&
    code.includes('for(')
  ) {

    performance -= 10;
  }

  performance =
    Math.max(
      0,
      Math.min(100, performance)
    );

  // SECURITY

  let security = 90;

  if (
    code.includes('gets(')
  ) {

    security -= 40;
  }

  if (
    code.includes('strcpy(')
  ) {

    security -= 20;
  }

  security =
    Math.max(
      0,
      Math.min(100, security)
    );

  // OVERALL

  const overall = Math.round(

    (
      readability * 0.25 +
      maintainability * 0.25 +
      complexity * 0.20 +
      performance * 0.15 +
      security * 0.15
    )
  );

  return {

    overall,

    complexity:
      Math.round(complexity),

    readability:
      Math.round(readability),

    maintainability:
      Math.round(maintainability),

    performance:
      Math.round(performance),

    security:
      Math.round(security)
  };
}

/* =========================
   MAIN COMPILER ENDPOINT
========================= */

app.post(
  '/api/compile',

  async (req, res) => {

    try {

      const { code } =
        req.body;

      if (!code) {

        return res.status(400).json({

          error:
            'No code provided'
        });
      }

      // TEMP FILE

      const tempFile =
        path.join(
          __dirname,
          `temp_${Date.now()}.c`
        );

      fs.writeFileSync(
        tempFile,
        code
      );

      try {

        // GCC COMPILE

        const {
          stderr
        } =
          await executeCommand(

            `gcc -Wall -Wextra -Wconversion "${tempFile}" -o "${tempFile}.out"`

          );

        const errors =
          parseErrors(stderr);

        const optimizations =
          analyzeOptimizations(code);

        const quality =
          calculateQualityMetrics(
            code,
            errors
          );

        return res.json({

          errors,

          optimizations,

          quality,

          compiled:
            errors.filter(
              e => e.type === 'error'
            ).length === 0
        });

      } finally {

        // CLEANUP

        try {

          if (
            fs.existsSync(tempFile)
          ) {

            fs.unlinkSync(tempFile);
          }

          if (
            fs.existsSync(
              `${tempFile}.out`
            )
          ) {

            fs.unlinkSync(
              `${tempFile}.out`
            );
          }

        } catch {}
      }

    } catch (error) {

      return res.status(500).json({

        error:
          error.message,

        errors: [
          {
            line: 1,
            column: 1,
            type: 'error',
            message:
              error.message,
            suggestion:
              'Check backend configuration'
          }
        ],

        optimizations: [],

        quality: {
          overall: 0,
          complexity: 0,
          readability: 0,
          maintainability: 0,
          performance: 0,
          security: 0
        }
      });
    }
  }
);

/* =========================
   START SERVER
========================= */

app.listen(PORT, () => {

  console.log(
    `✓ Compiler service running on http://localhost:${PORT}`
  );

});