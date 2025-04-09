#include <stdio.h>
#include <ctype.h>
#include "../include/parser.h"
#include "../include/generate.h"
#include "../include/io_utils.h"

int main(char* argv[]) {
    Scanner scanner;
    Trie trie;
    Parser parser;

    size_t size;
    char* source = read_stdin_to_end(&size);
    if (!source) {
        fprintf(stderr, "Error reading input\n");
     
        return 1;
    }
    
    create_scanner(&scanner, source);

    
    create_trie(&trie); 
    
    trie_insert(&trie, "begin", false, TOKEN_BEGIN);
    trie_insert(&trie, "end", false, TOKEN_END);
    trie_insert(&trie, "read", false, TOKEN_READ);
    trie_insert(&trie, "write", false, TOKEN_WRITE);
    
    init_parser(&parser, &scanner, &trie);
    
    ASTNode* ast = parse_program(&parser);
    printf("\n--- AST ---\n");
    print_ast(ast, 0);
    printf("\n--- END AST ---\n");

    generate_code(ast, &trie, "output.asm");
    assemble_and_run("output.asm");
    
    return 0;
}