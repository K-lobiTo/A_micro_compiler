#ifndef NODE_H
#define NODE_H

typedef struct TrieNode {
    struct TrieNode *children[26];
    int isEnd;
}TrieNode;

TrieNode* create_node();
void free_node(TrieNode *node);




#endif