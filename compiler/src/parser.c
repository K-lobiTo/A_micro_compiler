#include "../../include/parser.h"
#include "../../include/ast.h"

#include <stdio.h>
#include <stdlib.h>

void parser_init(Parser* p, Scanner* s) {
    p->scanner = s;
    p->current_token = next_token(s);
}

void match(Parser* p, TokenType expected_type) {
    if (p->current_token.type == expected_type) {
        printf("Matched %s\n", token_type_to_string(expected_type));
        p->current_token = next_token(p->scanner);
    } else {
        fprintf(stderr, "Syntax error: Expected %s, got %s\n",
                token_type_to_string(expected_type),
                token_type_to_string(p->current_token.type));
        exit(1);
    }
}
            // char var[33];
            // strcopy(var, p->current_token.lexeme);

ASTNode* parse_statement(Parser* p) {
    switch (p->current_token.type) {
        case TOKEN_ID: {
            char var_name[33];
            strcpy(var_name, p->current_token.lexeme);
            match(p, TOKEN_ID);
            if(p->current_token.type == TOKEN_ASSIGN){ // Assignment (declaration + initialization)
                match(p, TOKEN_ASSIGN);
                ASTNode* expr = parse_expression(p);
                match(p, TOKEN_SEMICOLON);
                return ast_new_declaration(var_name, expr);
            } else{// else: it's just declaration;
                match(p, TOKEN_SEMICOLON);
                return ast_new_declaration(var_name, NULL);
            }
        }
        case TOKEN_READ: {
            match(p, TOKEN_READ);
            char var_name[33];
            strcpy(var_name, p->current_token.lexeme);
            match(p, TOKEN_ID);
            match(p, TOKEN_SEMICOLON);
            return ast_new_read(var_name);
        }
        case TOKEN_WRITE: {
            match(p, TOKEN_WRITE);
            ASTNode* expr = parse_expression(p);
            match(p, TOKEN_SEMICOLON);
            return ast_new_write(expr);
        }
        case TOKEN_COMMENT: {
            match(p, TOKEN_COMMENT);
            return NULL;
        }
        default:
            fprintf(stderr, "Invalid statement at token%s\n", token_type_to_string(p->current_token.type));
            exit(1);
    }
}

ASTNode* parse_expression(Parser* p) {
    ASTNode* left;
    if (p->current_token.type == TOKEN_ID) {
        left = ast_new_variable(p->current_token.lexeme);
        match(p, TOKEN_ID);
        
    } else if(p->current_token.type == TOKEN_INT){
        match(p, TOKEN_INT);
    } else {
        fprintf(stderr, "Invalid expression\n");
        exit(1);
    }
    if (p->current_token.type == TOKEN_PLUS) {
        match(p, p->current_token.type);
        ASTNode * right = parse_expression(p);
        ast_add_binop(left, "+", right);
    }
    return left;
}

ASTNode* parse_program(Parser* p) {
    ASTNode* program = ast_new_program();
    while (p->current_token.type == TOKEN_COMMENT) {
        match(p, TOKEN_COMMENT);
    } 
    match(p, TOKEN_BEGIN);
    while (p->current_token.type != TOKEN_END) {
        ASTNode* stmt = parse_statement(p);
        if(stmt) ast_add_statement(program, stmt); //to skip NULL (comments)
    }
    match(p, TOKEN_END);
    while (p->current_token.type == TOKEN_COMMENT) {
        match(p, TOKEN_COMMENT);
    } 
    return program;
}