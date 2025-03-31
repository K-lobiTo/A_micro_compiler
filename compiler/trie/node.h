#ifndef nodeh
#define nodeh

typedef struct TrieNode {
    struct TrieNode *children[26];
    int isEnd;
}TrieNode;

TrieNode* create_node();
void free_node(TrieNode *node);




#endif