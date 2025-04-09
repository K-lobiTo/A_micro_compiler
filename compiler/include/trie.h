#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include "token.h"

#define ALPHABET_SIZE 37 // 26 letters + 10 digits + 1 underscore

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_end;
    TokenType type;
} TrieNode;

typedef struct Trie {
    TrieNode* root;
} Trie;

void create_trie(Trie* trie);
void trie_insert(Trie* trie, char* word, bool is_id, TokenType type);
TokenType trie_search(Trie* trie, char* word);

#endif