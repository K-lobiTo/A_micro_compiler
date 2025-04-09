#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/trie.h"

TrieNode* trie_node_create() {
    TrieNode* node = malloc(sizeof(TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) node->children[i] = NULL;
    node->is_end = false;
    node->type = TOKEN_ERROR;
    return node;
}

void create_trie(Trie* trie) { trie->root = trie_node_create(); }

int char_to_index(char c) {
    if (islower(c)) return c - 'a';
    if (isdigit(c)) return 26 + (c - '0');
    if (c == '_') return 36;
    return -1;
}

void trie_insert(Trie* trie, char* word, bool is_id, TokenType type) {
    TrieNode* current = trie->root;
    for (int i = 0; word[i]; i++) {
        int idx = char_to_index(word[i]);
        if (idx == -1) return;
        if (!(current->children[idx])) current->children[idx] = trie_node_create();
        current = current->children[idx];
    }
    current->is_end = is_id;
    current->type = type;
}

TokenType trie_search(Trie* trie, char* word) {
    TrieNode* current = trie->root;
    for (int i = 0; word[i]; i++) {
        int idx = char_to_index(word[i]);
        if (idx == -1 || !(current->children[idx])) return TOKEN_ERROR;
        current = current->children[idx];
    }
    return current->type;
}