#ifndef AST_H
#define AST_H

typedef enum NodeType {
    PROGRAM_NODE,
    STMT_NODE,
    ASSIGN_NODE,
    READ_NODE,
    WRITE_NODE,
    EXPR_NODE,
    ID_NODE,
    VALUE_NODE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct { struct ASTNode *first; struct ASTNode *rest; } stmt;
        struct { char *id; struct ASTNode *expr; } assign;
        struct { char *id; } read;
        struct { struct ASTNode *expr; } write;
        struct { 
            struct ASTNode *left;
            struct ASTNode *right;
        } expr;
        int value;
        char *id;
    };
} ASTNode;

ASTNode* create_program(ASTNode *stmts);
ASTNode* create_stmt_seq(ASTNode *first, ASTNode *rest);
ASTNode* create_assign(char *id, ASTNode *expr);
ASTNode* create_read(char *id);
ASTNode* create_write(ASTNode *expr);
ASTNode* create_id(char *name);
ASTNode* create_value(int value);
ASTNode* create_plus(ASTNode *left, ASTNode *right);

void print_ast(ASTNode* node, int depth);

#endif