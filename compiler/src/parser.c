#include "../../include/parser.h"
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

void parse_statement(Parser* p) {
    switch (p->current_token.type) {
        case TOKEN_ID: {
            match(p, TOKEN_ID);
            if(p->current_token.type == TOKEN_ASSIGN){
                match(p, TOKEN_ASSIGN);
                parse_expression(p);
            } // else: it's just declaration;
            match(p, TOKEN_SEMICOLON);
            break;
        }
        case TOKEN_READ: {
            match(p, TOKEN_READ);
            match(p, TOKEN_ID);
            match(p, TOKEN_SEMICOLON);
            break;
        }
        case TOKEN_WRITE: {
            match(p, TOKEN_WRITE);
            parse_expression(p);
            match(p, TOKEN_SEMICOLON);
            break;
        }
        case TOKEN_COMMENT: {
            match(p, TOKEN_COMMENT);
            break;
        }
        default:
            fprintf(stderr, "Invalid statement\n");
            exit(1);
    }
}

void parse_expression(Parser* p) {
    if (p->current_token.type == TOKEN_ID || p->current_token.type == TOKEN_INT) {
        Token t = p->current_token;
        match(p, t.type);
        
        if (p->current_token.type == TOKEN_PLUS) {
            match(p, p->current_token.type);
            parse_expression(p);
        }
    } else {
        fprintf(stderr, "Invalid expression\n");
        exit(1);
    }
}

void parse_program(Parser* p) {
    // maybe we should match comments here, before the begin;
    match(p, TOKEN_BEGIN);
    while (p->current_token.type != TOKEN_END) {
        parse_statement(p);
    }
    match(p, TOKEN_END);
}