#ifndef SCANNER_H
#define SCANNER_H

#include "trie.h"

typedef struct {
    TokenType type;
    char lexeme[33];
} Token;

typedef struct {
    char* source;
    int current;
    int size;
} Scanner;

void create_scanner(Scanner* scanner, char* source);
Token next_token(Scanner* scanner, Trie* trie);
const char* token_type_to_string(TokenType type);

#endif