#include "../../include/trie/node.h"
#include <stdlib.h>

TrieNode* create_node(){
    TrieNode *node = malloc(sizeof(TrieNode));
    for(int i = 0; i < 26; ++i)node->children[i] = NULL;
    node->isEnd = 0;
    return node;
}

void free_node(TrieNode *node){
    if(!node)return;
    for(int i = 0; i < 26; ++i)free_node(node->children[i]);
    free(node);
}