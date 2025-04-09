#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "trie.h"

void generate_code(ASTNode* node, Trie* trie, const char* output_filename);
void assemble_and_run(const char* asm_file);
const char* node_type_to_string(NodeType);

#endif