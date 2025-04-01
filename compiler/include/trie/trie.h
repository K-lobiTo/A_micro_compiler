#ifndef trieh
#define trieh

#include "node.h"

typedef struct {
    TrieNode *root;
}Trie;

void insert(Trie *trie, const char *word);
int search(Trie *trie, const char *word);





#endif