#include "../include/scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static char peek(Scanner* s) {
    return (s->pos < s->len) ? s->source[s->pos] : '\0';
}

static char advance(Scanner* s) {
    return (s->pos < s->len) ? s->source[s->pos++] : '\0';
}

static void skip_whitespace(Scanner* s) {
    while (isspace(peek(s))) advance(s);
}

static Token scan_digits(Scanner* s) {
    Token token = {TOKEN_INT, {0}};
    int i = 0;
    while (isdigit(peek(s))) {
        token.lexeme[i++] = advance(s);
    }
    token.lexeme[i] = '\0';
    return token;
}

// Not really necessary anymore
// int isletter(char c){
//     return (c>=97 && c<=122);
// }

// static Token scan_variable_id(Scanner* s) {
//     Token token = {TOKEN_ID, {0}};
//     int i = 0;
//     while (isletter(peek(s))) {
//         token.lexeme[i++] = advance(s);
//     }
//     token.lexeme[i] = '\0';
//     return token;
// }

// Main scanner function
Token next_token(Scanner* s) {
    skip_whitespace(s);
    Token token = {TOKEN_ERROR, {0}};

    if (peek(s) == '\0') {
        token.type = TOKEN_EOF;
    } else if (isdigit(peek(s))) {
        token = scan_digits(s);
    }
    else if (isalpha(peek(s))) { //currently includes uppercase letters, numbers too (not at start)
        char ident[32];
        int i = 0;
        while (isalnum(peek(s)) && i < 31) { // token lexeme size might need to be changed  (+1)
            ident[i++] = advance(s);
        }
        ident[i] = '\0';

        if (strcmp(ident, "begin") == 0) token.type = TOKEN_BEGIN;
        else if (strcmp(ident, "end") == 0) token.type = TOKEN_END;
        else if (strcmp(ident, "write") == 0) token.type = TOKEN_WRITE;
        else if (strcmp(ident, "read") == 0) token.type = TOKEN_READ;
        else if (strcmp(ident, "i") == 0) token.type = TOKEN_INT_DCL;
        else {
            token.type = TOKEN_ID;
            strncpy(token.lexeme, ident, 31); // token lexeme size might need to be changed  (+1)
        }
    }
    else {
        switch (advance(s)) {
            case '=': token.type = TOKEN_ASSIGN; break;
            case '+': token.type = TOKEN_PLUS; break;
            case ';': token.type = TOKEN_SEMICOLON; break;
            default: token.type = TOKEN_ERROR;
        }
    }
    return token;
}

void scanner_init(Scanner* s, const char* source) {
    s->source = source;
    s->pos = 0;
    s->len = strlen(source);
}