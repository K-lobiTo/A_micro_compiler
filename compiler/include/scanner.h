
#ifndef SCANNER_H
#define SCANNER_H
#include <stddef.h>

typedef enum { // Maybe something is missing [check later] 
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_INT,
    TOKEN_SEMICOLON,
    TOKEN_ID,
    TOKEN_READ,
    TOKEN_WRITE,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_EOF,
    TOKEN_COMMENT,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[33]; // extra char for '\0'
} Token;

typedef struct {
    const char* source;
    size_t pos;
    size_t len;
} Scanner;

void scanner_init(Scanner* s, const char* source);

Token next_token(Scanner* s);

#endif