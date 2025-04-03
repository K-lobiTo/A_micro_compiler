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


const char* token_type_to_string(TokenType type) {
    static const char* names[] = {
        [TOKEN_BEGIN] = "TOKEN_BEGIN",
        [TOKEN_END] = "TOKEN_END",
        [TOKEN_ID] = "TOKEN_ID",
        [TOKEN_INT] = "TOKEN_INT",
        [TOKEN_READ] = "TOKEN_READ",
        [TOKEN_WRITE] = "TOKEN_WRITE",
        [TOKEN_ASSIGN] = "TOKEN_ASSIGN",
        [TOKEN_PLUS] = "TOKEN_PLUS",
        [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
        [TOKEN_COMMENT] = "TOKEN_COMMENT",
        [TOKEN_EOF] = "TOKEN_EOF",
        [TOKEN_ERROR] = "TOKEN_ERROR"
    };

    // Handle invalid type values
    if (type >= sizeof(names)/sizeof(names[0]) || names[type] == NULL) {
        return "TOKEN_UNKNOWN";
    }
    return names[type];
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
        while (isalnum(peek(s)) && i < 32) {
            ident[i++] = advance(s);
        }
        ident[i] = '\0';

        if (strcmp(ident, "begin") == 0) token.type = TOKEN_BEGIN;
        else if (strcmp(ident, "end") == 0) token.type = TOKEN_END;
        else if (strcmp(ident, "write") == 0) token.type = TOKEN_WRITE;
        else if (strcmp(ident, "read") == 0) token.type = TOKEN_READ;
        else {
            token.type = TOKEN_ID;
            // strncpy(token.lexeme, ident, 32); // [this was passed down momentarily]
        }
        strncpy(token.lexeme, ident, 32); // saving lexeme for all (just for debbugin facilities)
    }
    else {
        switch (advance(s)) {
            case '-': {
                token.type = TOKEN_COMMENT;
                strncpy(token.lexeme, "-", 32);
                while (peek(s)!='\n' && peek(s)!='\0')advance(s);
                break;
            }
            case '=': token.type = TOKEN_ASSIGN; strncpy(token.lexeme, "=", 32); break;
            case '+': token.type = TOKEN_PLUS; strncpy(token.lexeme, "+", 32); break;
            case ';': token.type = TOKEN_SEMICOLON; strncpy(token.lexeme, ";", 32);break;
            default: token.type = TOKEN_ERROR; strncpy(token.lexeme, "!ERR", 32); break;; 
        }
    }
    return token;
}

void scanner_init(Scanner* s, const char* source) {
    s->source = source;
    s->pos = 0;
    s->len = strlen(source);
}