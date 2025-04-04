

#include "../include/ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ---------------------------
// AST Node Constructors
// ---------------------------

ASTNode* ast_new_program() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PROGRAM;
    node->statements = NULL;
    node->statement_count = 0;
    return node;
}

ASTNode* ast_new_assignment(const char* var, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGN;
    strncpy(node->var_name, var, 31);
    node->var_name[32] = '\0';
    node->left = expr;  // Reusing 'left' for the expression
    return node;
}

ASTNode* ast_new_read(const char* var) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_READ;
    strncpy(node->var_name, var, 31);
    node->var_name[32] = '\0';
    return node;
}

ASTNode* ast_new_write(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_WRITE;
    node->left = expr;  // Reusing 'left' for the expression
    return node;
}

ASTNode* ast_new_variable(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ID;
    strncpy(node->var_name, name, 31);
    node->var_name[32] = '\0';
    return node;
}

ASTNode* ast_new_literal(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_LITERAL;
    node->int_value = value;
    return node;
}

ASTNode* ast_new_binop(ASTNode* left, char op, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINOP;
    node->left = left;
    node->right = right;
    node->op = op;
    return node;
}

ASTNode* ast_new_declaration(const char* var, ASTNode* initializer) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION;
    strncpy(node->decl.var_name, var, 31);
    node->decl.var_name[31] = '\0';
    node->decl.is_initialized = (initializer != NULL);
    node->decl.initializer = initializer;
    return node;
}
// ---------------------------
// AST Manipulation
// ---------------------------

void ast_add_statement(ASTNode* program, ASTNode* statement) {
    program->statements = realloc(program->statements, 
                                sizeof(ASTNode*) * (program->statement_count + 1));
    program->statements[program->statement_count++] = statement;
}

// ---------------------------
// AST Destruction
// ---------------------------

void ast_free(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case AST_BINOP:
            ast_free(node->left);
            ast_free(node->right);
            break;
        case AST_ASSIGN:
        case AST_WRITE:
            ast_free(node->left);  // Free the expression
            break;
        case AST_PROGRAM:
            for (size_t i = 0; i < node->statement_count; i++) {
                ast_free(node->statements[i]);
            }
            free(node->statements);
            break;
        default:
            break;  // No special cleanup for literals/variables
    }
    free(node);
}

// ---------------------------
// AST Debugging
// ---------------------------

static const char* ast_type_to_string(ASTNodeType type) {
    static const char* names[] = {
        [AST_PROGRAM] = "PROGRAM",
        [AST_ASSIGN] = "ASSIGN",
        [AST_READ] = "READ",
        [AST_WRITE] = "WRITE",
        [AST_ID] = "VAR",
        [AST_LITERAL] = "LITERAL",
        [AST_BINOP] = "BINOP"
    };
    return names[type];
}

void ast_print(ASTNode* node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case AST_LITERAL:
            printf("%s: %d\n", ast_type_to_string(node->type), node->int_value);
            break;
        case AST_ID:
            printf("%s: %s\n", ast_type_to_string(node->type), node->var_name);
            break;
        case AST_BINOP:
            printf("%s: '%c'\n", ast_type_to_string(node->type), node->op);
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            break;
        case AST_ASSIGN:
            printf("%s: %s\n", ast_type_to_string(node->type), node->var_name);
            ast_print(node->left, indent + 1);
            break;
        case AST_WRITE:
            printf("%s:\n", ast_type_to_string(node->type));
            ast_print(node->left, indent + 1);
            break;
        case AST_PROGRAM:
            printf("%s:\n", ast_type_to_string(node->type));
            for (size_t i = 0; i < node->statement_count; i++) {
                ast_print(node->statements[i], indent + 1);
            }
            break;
        default:
            printf("%s\n", ast_type_to_string(node->type));
    }
}