#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"

void match(Parser* p, TokenType expected) {
    while (p->current.type == TOKEN_COMMENT) p->current = next_token(p->scanner, p->trie);
    if (p->current.type != expected) {
        fprintf(stderr, "Error sintáctico: Esperaba %s, obtuvo %s\n",
            token_type_to_string(expected), token_type_to_string(p->current.type));
        exit(EXIT_FAILURE);
    }
    p->current = next_token(p->scanner, p->trie);
}

ASTNode* parse_program(Parser* p) {
    match(p, TOKEN_BEGIN);
    match(p, TOKEN_SEMICOLON);
    ASTNode* stmts = parse_stmts(p);
    match(p, TOKEN_END);
    match(p, TOKEN_SEMICOLON);
    match(p, TOKEN_EOF);
    return create_program(stmts);
}

ASTNode* parse_stmts(Parser* p) {
    while (p->current.type == TOKEN_COMMENT ||
            p->current.type == TOKEN_SEMICOLON)
        p->current = next_token(p->scanner, p->trie);
    
    if (p->current.type == TOKEN_ID || 
        p->current.type == TOKEN_READ || 
        p->current.type == TOKEN_WRITE) {
        
        ASTNode* stmt = parse_stmt(p);
        return create_stmt_seq(stmt, parse_stmts(p));
    }
    return NULL;
}

ASTNode* parse_stmt(Parser* p) {
    switch(p->current.type) {
        case TOKEN_ID: return parse_assignment(p);
        case TOKEN_READ: return parse_read(p);
        case TOKEN_WRITE: return parse_write(p);
        default:
            fprintf(stderr, "Declaración inválida\n");
            exit(EXIT_FAILURE);
    }
}

ASTNode* parse_assignment(Parser* p) {
    char* id = strdup(p->current.lexeme);
    match(p, TOKEN_ID);
    trie_insert(p->trie, id, true, TOKEN_ID);

    if(p->current.type == TOKEN_SEMICOLON){
        match(p, TOKEN_SEMICOLON);
        ASTNode* zero = create_value(0);
        return create_assign(id, zero);
    }

    match(p, TOKEN_ASSIGN);
    ASTNode* expr = parse_expr(p);
    match(p, TOKEN_SEMICOLON);
    return create_assign(id, expr);
}

ASTNode* parse_read(Parser* p) {
    match(p, TOKEN_READ);
    char* id = strdup(p->current.lexeme);
    match(p, TOKEN_ID);
    if (trie_search(p->trie, id) == TOKEN_ERROR) {
        fprintf(stderr, "Error: Variable '%s' no declarada\n", id);
        exit(EXIT_FAILURE);
    }
    // if (trie_search(p->trie, id) == TOKEN_ERROR) trie_insert(p->trie, id, true, TOKEN_ID);
    match(p, TOKEN_SEMICOLON);
    return create_read(id);
}

ASTNode* parse_write(Parser* p) {
    match(p, TOKEN_WRITE);
    ASTNode* expr = parse_expr(p);
    if (expr->type == ID_NODE) {
        if (trie_search(p->trie, expr->id) == TOKEN_ERROR) {
            fprintf(stderr, "Error: Variable '%s' no declarada\n", expr->id);
            exit(EXIT_FAILURE);
        }
    }
    match(p, TOKEN_SEMICOLON);
    return create_write(expr);
}

ASTNode* parse_expr(Parser* p) {
    ASTNode* left = parse_term(p);
    while (p->current.type == TOKEN_PLUS) {
        match(p, TOKEN_PLUS);
        ASTNode* right = parse_term(p);
        left = create_plus(left, right);
    }
    return left;
}

ASTNode* parse_term(Parser* p) {
    if (p->current.type == TOKEN_ID) {
        char* id = strdup(p->current.lexeme); 
        if (trie_search(p->trie, id) == TOKEN_ERROR) {
            fprintf(stderr, "Error: Variable '%s' no declarada\n", p->current.lexeme);
            exit(EXIT_FAILURE);
        }
        match(p, TOKEN_ID);
        return create_id(id);
    } else if (p->current.type == TOKEN_VALUE) {
        int value = atoi(p->current.lexeme);
        match(p, TOKEN_VALUE);
        return create_value(value);
    } else {
        fprintf(stderr, "Se esperaba identificador o número\n");
        exit(EXIT_FAILURE);
    }
}

void init_parser(Parser* p, Scanner* s, Trie* t) {
    p->scanner = s;
    p->trie = t;
    p->current = next_token(s, t);
}