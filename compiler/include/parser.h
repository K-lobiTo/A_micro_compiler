#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ast.h"

typedef struct Parser {
    Scanner* scanner;
    Trie* trie;
    Token current;
} Parser;

void init_parser(Parser* p, Scanner* s, Trie* t);
void match(Parser* p, TokenType expected_type);

ASTNode* parse_program(Parser* p);
ASTNode* parse_stmts(Parser* p);
ASTNode* parse_stmt(Parser* p);
ASTNode* parse_assignment(Parser* p);
ASTNode* parse_read(Parser* p);
ASTNode* parse_write(Parser* p);
ASTNode* parse_expr(Parser* p);
ASTNode* parse_term(Parser* p);

#endif