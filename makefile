# Makefile for IntelliCompile Lexical Analyzer

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
FLEX = flex
LEXER = lexer
TARGET = intelli_lexer

# Source files
LEX_SOURCE = lexer.l
LEX_OUTPUT = lex.yy.c
SOURCES = token.c ai_error_detector.c $(LEX_OUTPUT)
OBJECTS = $(SOURCES:.c=.o)
HEADERS = token.h

# Default target
all: $(TARGET)

# Build the lexer
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

# Generate lexer from .l file
$(LEX_OUTPUT): $(LEX_SOURCE)
	$(FLEX) -o $@ $<

# Compile object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(LEX_OUTPUT) $(TARGET)

# Test with sample file
test: $(TARGET)
	./$(TARGET) test_sample.c

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install flex bison build-essential

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the lexer"
	@echo "  clean        - Remove build files"
	@echo "  test         - Test with sample file"
	@echo "  install-deps - Install dependencies"
	@echo "  help         - Show this help"

.PHONY: all clean test install-deps help