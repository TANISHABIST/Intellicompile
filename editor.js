import React, { useRef, useEffect } from 'react';
import Editor from '@monaco-editor/react';

const CodeEditor = ({ value, onChange, errors }) => {
  const editorRef = useRef(null);

  const handleEditorDidMount = (editor, monaco) => {
    editorRef.current = editor;
    
    // Configure C language support
    monaco.languages.register({ id: 'c' });
    
    // Set up C language syntax highlighting
    monaco.languages.setMonarchTokensProvider('c', {
      tokenizer: {
        root: [
          [/\b(int|float|char|void|if|else|while|for|do|return|break|continue|switch|case|default|sizeof|struct|typedef)\b/, 'keyword'],
          [/\b[a-zA-Z_][a-zA-Z0-9_]*\b/, 'identifier'],
          [/\d+\.\d+([eE][+-]?\d+)?[fF]?/, 'number.float'],
          [/\b\d+\b/, 'number'],
          [/\"([^\"\\\n]|\\.|\\\n)*\"/, 'string'],
          [/\'([^\'\\\n]|\\.|\\\n)\'/, 'string'],
          [/[+\-*/%=<>!&|^~]/, 'operator'],
          [/[();,\[\]{}]/, 'delimiter'],
          [/\/\/.*$/, 'comment'],
          [/\/\*[\s\S]*?\*\//, 'comment'],
        ],
      },
    });

    // Set up theme
    monaco.editor.defineTheme('intelli-theme', {
      base: 'vs',
      inherit: true,
      rules: [
        { token: 'keyword', foreground: '0000ff' },
        { token: 'identifier', foreground: '000000' },
        { token: 'number', foreground: '098658' },
        { token: 'string', foreground: 'a31515' },
        { token: 'comment', foreground: '008000', fontStyle: 'italic' },
        { token: 'operator', foreground: '000000' },
        { token: 'delimiter', foreground: '000000' },
      ],
      colors: {
        'editor.background': '#ffffff',
        'editor.foreground': '#000000',
        'editor.lineHighlightBackground': '#f0f0f0',
        'editorCursor.foreground': '#000000',
        'editorWhitespace.foreground': '#cccccc',
      },
    });

    // Apply theme and language
    editor.setModel(editor.getModel(), monaco.editor.createModel(value, 'c'));
    monaco.editor.setTheme('intelli-theme');
  };

  // Update error decorations
  useEffect(() => {
    if (editorRef.current && errors.length > 0) {
      const decorations = errors.map(error => ({
        range: {
          startLineNumber: error.line,
          startColumn: error.column,
          endLineNumber: error.line,
          endColumn: error.column + 10,
        },
        options: {
          className: error.type === 'error' ? 'error-line' : 'warning-line',
          hoverMessage: { value: error.message },
          isWholeLine: true,
        },
      }));

      editorRef.current.deltaDecorations([], decorations);
    }
  }, [errors]);

  return (
    <Editor
      height="100%"
      defaultLanguage="c"
      value={value}
      onChange={onChange}
      onMount={handleEditorDidMount}
      options={{
        theme: 'intelli-theme',
        fontSize: 14,
        fontFamily: 'Monaco, Menlo, "Ubuntu Mono", monospace',
        lineNumbers: 'on',
        minimap: { enabled: true },
        scrollBeyondLastLine: false,
        wordWrap: 'on',
        automaticLayout: true,
        suggestOnTriggerCharacters: true,
        quickSuggestions: true,
        parameterHints: { enabled: true },
        folding: true,
        foldingStrategy: 'indentation',
        showFoldingControls: 'always',
        renderWhitespace: 'selection',
        bracketPairColorization: { enabled: true },
        guides: {
          indentation: true,
          bracketPairs: true,
        },
      }}
    />
  );
};

export default CodeEditor;