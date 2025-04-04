#ifndef AST.H
#define AST.H

#include "./trie/trie.h"
#include "parser.h"

typedef enum {
    AST_PROGRAM,
    AST_LITERAL,
    AST_BINOP,
    AST_ASSIGN,
    AST_READ,
    AST_WRITE,
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_ID
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        // For literals (int values)
        int int_value;
        
        // For variables
        char var_name[33];

        // For declarations
        struct {
            char var_name[33];
            int is_initialized;  // 0 = declaration only, 1 = with assignment
            ASTNode* initializer;  // NULL if just declaration
        } decl;
        
        // For binary operations 
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char op; // '+'
        };
        
        // For statements
        struct {
            struct ASTNode** statements; // Program or block
            size_t statement_count;
        };

    };
} ASTNode;

ASTNode* ast_new_program();
ASTNode* ast_new_assignment(char* var, ASTNode* expr);
ASTNode* ast_new_declaration(const char* var, ASTNode* initializer);
ASTNode* ast_new_literal(int value);
ASTNode* ast_new_variable(char* name);
ASTNode* ast_new_binop(ASTNode* left, char op, ASTNode* right);
void ast_free(ASTNode* node);


#endif




