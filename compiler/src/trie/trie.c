#include "../../include/trie/trie.h"
#include <string.h> //hopefully we can use this lib for the assignment 

void insert(Trie *trie, const char *word){
    TrieNode *current = trie->root;
    for (int i = 0; i < strlen(word); ++i){
        int idx = word[i]-'a';
        if(!current->children[idx]){
            current->children[idx] = create_node();
        }
        current = current->children[idx];
    }
    current->isEnd = 1;
}

// this could be wrong 
int search(Trie *trie, const char *word){
    TrieNode *current = trie->root;
    for (int i = 0; i < strlen(word); ++i){
        int idx = word[i]-'a';
        if(!current->children[idx]){
            return 0;
        }
        current = current->children[idx];
    }
    return current->isEnd;
}