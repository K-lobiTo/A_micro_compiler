#include <ctype.h>
#include <string.h>
#include "../include/scanner.h"

static const char FINAL_CHAR = '\0';

char peek(Scanner* scanner) { return (scanner->current < scanner->size) ? scanner->source[scanner->current] : FINAL_CHAR; }
char advance(Scanner* scanner) { return (scanner->current < scanner->size) ? scanner->source[scanner->current++] : FINAL_CHAR; }

Token scan_digits(Scanner* s) {
    Token token = {TOKEN_VALUE, {0}};
    int i = 0;
    while (isdigit(peek(s)) && i < 32) {
        token.lexeme[i++] = advance(s);
    }
    token.lexeme[i] = '\0';
    return token;
}

Token scan_identifier(Scanner* s, Trie* trie) {
    Token token = {TOKEN_ERROR, {0}};
    int i = 0;
    char c = peek(s);
    while ((islower(c) || isdigit(c) || c == '_') && i < 32) {
        token.lexeme[i++] = advance(s);
        c = peek(s);
    }
    token.lexeme[i] = '\0';
    token.type = (trie_search(trie, token.lexeme) == TOKEN_ERROR) ? TOKEN_ID : trie_search(trie, token.lexeme);

    return token;
}

static Token process_comment(Scanner* s) {
    advance(s);
    while (peek(s) != '\n' && peek(s) != '\0') advance(s);
    return (Token){TOKEN_COMMENT, {0}};
}


Token next_token(Scanner* s, struct Trie* trie) {
    while (isspace(peek(s))) advance(s);
    char c = peek(s);

    if (c == '\0') return (Token){TOKEN_EOF, {0}};
    if (isdigit(c)) return scan_digits(s);
    if (islower(c) || c == '_') return scan_identifier(s, trie);
    if (c == '-') return process_comment(s);

    switch (advance(s)) {
        case '=': return (Token){TOKEN_ASSIGN, .lexeme = "="};
        case '+': return (Token){TOKEN_PLUS, .lexeme = "+"};
        case ';': return (Token){TOKEN_SEMICOLON, .lexeme = ";"};
        default: return (Token){TOKEN_ERROR, {c}}; // si no, error token.lexeme[0] = c
    }
}

void create_scanner(Scanner* scanner, char* source) {
    scanner->source = source;
    scanner->current = 0;
    scanner->size = strlen(source);
}

const char* token_type_to_string(TokenType type) {
    static const char* names[] = {
        [TOKEN_BEGIN] = "TOKEN_BEGIN",
        [TOKEN_END] = "TOKEN_END",
        [TOKEN_READ] = "TOKEN_READ",
        [TOKEN_WRITE] = "TOKEN_WRITE",
        [TOKEN_ID] = "TOKEN_ID",
        [TOKEN_VALUE] = "TOKEN_VALUE",
        [TOKEN_ASSIGN] = "TOKEN_ASSIGN",
        [TOKEN_PLUS] = "TOKEN_PLUS",
        [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
        [TOKEN_COMMENT] = "TOKEN_COMMENT",
        [TOKEN_ERROR] = "TOKEN_ERROR",
        [TOKEN_EOF] = "TOKEN_EOF"
    };
    return names[type];
}