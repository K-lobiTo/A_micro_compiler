#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef struct {
    Scanner* scanner;
    Token current_token;
} Parser;

void parser_init(Parser* p, Scanner* s);
void parse_program(Parser* p);
void match(Parser* p, TokenType expected_type);

#endif