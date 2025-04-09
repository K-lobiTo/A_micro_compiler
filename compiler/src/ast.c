#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/ast.h"

ASTNode* create_program(ASTNode* stmts) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = PROGRAM_NODE;
    node->stmt.first = stmts;
    node->stmt.rest = NULL;
    return node;
}

ASTNode* create_stmt_seq(ASTNode* first, ASTNode* rest) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = STMT_NODE;
    node->stmt.first = first;
    node->stmt.rest = rest;
    return node;
}

ASTNode* create_assign(char* id, ASTNode* expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = ASSIGN_NODE;
    node->assign.id = id;
    node->assign.expr = expr;
    return node;
}

ASTNode* create_read(char *id) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = READ_NODE;
    node->read.id = id;
    return node;
}

ASTNode* create_write(ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = WRITE_NODE;
    node->write.expr = expr;
    return node;
}

ASTNode* create_id(char* id) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = ID_NODE;
    node->id = id;
    return node;
}

ASTNode* create_value(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = VALUE_NODE;
    node->value = value;
    return node;
}

ASTNode* create_plus(ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = EXPR_NODE;
    node->expr.left = left;
    node->expr.right = right;
    return node;
}

void print_ast(ASTNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch(node->type) {
        case PROGRAM_NODE:
            printf("Program\n");
            print_ast(node->stmt.first, depth + 1);
            break;
            
        case STMT_NODE:
            printf("Statement Sequence\n");
            print_ast(node->stmt.first, depth + 1);
            print_ast(node->stmt.rest, depth);
            break;
            
        case ASSIGN_NODE:
            printf("Assign: %s\n", node->assign.id);
            print_ast(node->assign.expr, depth + 1);
            break;
            
        case READ_NODE:
            printf("Read: %s\n", node->read.id);
            break;
            
        case WRITE_NODE:
            printf("Write:\n");
            print_ast(node->write.expr, depth + 1);
            break;
            
        case EXPR_NODE:
            printf("Expression: %c\n", '+');
            print_ast(node->expr.left, depth + 1);
            print_ast(node->expr.right, depth + 1);
            break;
            
        case ID_NODE:
            printf("Identifier: %s\n", node->id);
            break;
            
        case VALUE_NODE:
            printf("Number: %d\n", node->value);
            break;
    }
}