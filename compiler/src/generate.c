#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/generate.h"
#include "../include/trie.h"


void generate_expr(ASTNode* node, FILE* out, Trie* trie) {
    // fprintf(stderr, "\n generate_expr : %s\n", node_type_to_string(node->type));
    // fprintf(stderr, "%d\n", node->value);
    switch(node->type) {
        case ID_NODE: {
            int idx = trie_search_idx(trie, node->id);
            fprintf(out, "    mov eax, [var_%d]\n", idx);
            break;
        }
        case VALUE_NODE:
            fprintf(out, "    mov eax, %d\n", node->value);
            break;
        case EXPR_NODE:
            generate_expr(node->expr.left, out, trie);
            fprintf(out, "    push eax\n");
            generate_expr(node->expr.right, out, trie);
            fprintf(out, "    pop ebx\n");
            fprintf(out, "    add eax, ebx\n");
            break;
        default:
            fprintf(stderr, "Expresión inválida\n");
            exit(EXIT_FAILURE);
    }
}

void generate_assign(ASTNode* node, FILE* out, Trie* trie) {
    int idx = trie_search_idx(trie, node->assign.id);
    generate_expr(node->assign.expr, out, trie);
    fprintf(out, "    mov [var_%d], eax\n", idx);
}

void generate_read(ASTNode* node, FILE* out, Trie* trie) {
    int idx = trie_search_idx(trie, node->read.id);
    
    fprintf(out, "    ;; READ %s\n", node->read.id);
    fprintf(out, "    mov eax, 3       ; sys_read\n");
    fprintf(out, "    mov ebx, 0       ; stdin\n");
    fprintf(out, "    mov ecx, buffer\n");
    fprintf(out, "    mov edx, 20\n");
    fprintf(out, "    int 0x80\n");
    
    // Convertir a entero
    fprintf(out, "    call atoi\n");
    fprintf(out, "    mov [var_%d], eax\n", idx);
}

void generate_write(ASTNode* node, FILE* out, Trie* trie) {
    fprintf(out, "    ;; WRITE\n");
    generate_expr(node->write.expr, out, trie);
    
        
    // Convert to string
    fprintf(out, "    call itoa\n");
    fprintf(out, "    mov ecx, eax       ; String pointer\n");
    fprintf(out, "    mov ebx, 1         ; stdout\n");
    fprintf(out, "    mov eax, 4         ; sys_write\n");
    fprintf(out, "    int 0x80           ; Output number\n");
    
    // Add newline
    fprintf(out, "    mov eax, 4         ; sys_write\n");
    fprintf(out, "    mov ebx, 1         ; stdout\n");
    fprintf(out, "    mov ecx, newline\n");
    fprintf(out, "    mov edx, 1         ; Length 1\n");
    fprintf(out, "    int 0x80\n");

}

void generate_stmts(ASTNode* node, FILE* out, Trie* trie, int iter) {
    while(node) {
        // fprintf(stderr, "\n iteracion: %d\n", iter);
        // fprintf(stderr, " Node type before change in end of generate_stmts: %s\n", node_type_to_string(node->type));
        if (node->type != STMT_NODE) {
            // Handle single statement
            switch(node->type) {
                case ASSIGN_NODE: generate_assign(node, out, trie); break;
                case READ_NODE: generate_read(node, out, trie); break;
                case WRITE_NODE: generate_write(node, out, trie); break;
                default: 
                    fprintf(stderr, "Invalid statement\n");
                    exit(EXIT_FAILURE);
            }
            break;
        }
        
        // Handle statement sequence
        generate_stmts(node->stmt.first, out, trie, iter + 1);
        node = node->stmt.rest;
    }
}

void generate_code(ASTNode* node, Trie* trie, const char* output_filename) {
    FILE* out = fopen(output_filename, "w");
    if (!out) {
        perror("Error al crear archivo de salida");
        exit(EXIT_FAILURE);
    }

    // Cabecera del ensamblador
    fprintf(out, "section .data\n");
    
    // Variables
    for(int i = 0; i < trie->firstIdx; i++) {
        fprintf(out, "var_%d: dd 0\n", i);
    }
    
    fprintf(out, "buffer: times 20 db 0\n");
    fprintf(out, "newline: db 10\n\n");

    fprintf(out, "section .text\n");
    fprintf(out, "global _start\n\n");

    // Funciones de conversión
    fprintf(out, "atoi:\n");
    fprintf(out, "    xor eax, eax\n");
    fprintf(out, "    xor ecx, ecx\n");
    fprintf(out, "    mov esi, buffer\n");
    fprintf(out, ".convert:\n");
    fprintf(out, "    movzx edx, byte [esi + ecx]\n");
    fprintf(out, "    cmp dl, 10\n");
    fprintf(out, "    je .done\n");
    fprintf(out, "    sub dl, '0'\n");
    fprintf(out, "    imul eax, 10\n");
    fprintf(out, "    add eax, edx\n");
    fprintf(out, "    inc ecx\n");
    fprintf(out, "    jmp .convert\n");
    fprintf(out, ".done:\n");
    fprintf(out, "    ret\n\n");

    
    fprintf(out, "itoa:\n");
    fprintf(out, "    mov esi, buffer + 19\n");
    fprintf(out, "    mov byte [esi], 0\n");
    fprintf(out, "    mov ebx, 10\n");
    fprintf(out, ".loop:\n");
    fprintf(out, "    dec esi\n");
    fprintf(out, "    xor edx, edx\n");
    fprintf(out, "    div ebx\n");
    fprintf(out, "    add dl, '0'\n");
    fprintf(out, "    mov [esi], dl\n");
    fprintf(out, "    test eax, eax\n");
    fprintf(out, "    jnz .loop\n");
    fprintf(out, "    mov eax, esi\n");
    fprintf(out, "    mov edx, buffer + 19\n");  // Calculate length
    fprintf(out, "    sub edx, eax\n");
    fprintf(out, "    ret\n\n");

    fprintf(out, "_start:\n");
    
    generate_stmts(node->stmt.first, out, trie, 1);

    // Add pause before exit (Linux specific)
    fprintf(out, "    ;; Wait for keypress\n");
    fprintf(out, "    mov eax, 3       ; sys_read\n");
    fprintf(out, "    mov ebx, 0       ; stdin\n");
    fprintf(out, "    mov ecx, buffer  ; Dummy buffer\n");
    fprintf(out, "    mov edx, 1       ; Read 1 byte\n");
    fprintf(out, "    int 0x80\n");

    // Salida
    fprintf(out, "    ;; Exit\n");
    fprintf(out, "    mov eax, 1       ; sys_exit\n");
    fprintf(out, "    xor ebx, ebx\n");
    fprintf(out, "    int 0x80\n");

    fclose(out);
}

void assemble_and_run(const char* asm_file) {
    char obj_file[256];
    char exec_file[256];
    
    snprintf(obj_file, sizeof(obj_file), "%s.o", asm_file);
    snprintf(exec_file, sizeof(exec_file), "%s.out", asm_file);

    // 1. Ensamblar
    printf("Ensamblando %s...\n", asm_file);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "nasm -felf32 %s -o %s", asm_file, obj_file);
    int result = system(cmd);
    if (result != 0) {
        fprintf(stderr, "Error en ensamblado (código %d)\n", result);
        return;
    }

    // 2. Linkear
    printf("Linkeando %s...\n", obj_file);
    snprintf(cmd, sizeof(cmd), "ld -m elf_i386 %s -o %s", obj_file, exec_file);
    result = system(cmd);
    if (result != 0) {
        fprintf(stderr, "Error en linking (código %d)\n", result);
        return;
    }

    // 3. Ejecutar
    printf("Ejecutando %s...\n", exec_file);
    snprintf(cmd, sizeof(cmd), "./%s", exec_file);
    result = system(cmd);
    
    // 4. Mostrar resultado
    printf("\nEl programa terminó con código %d\n", result);
    
    // 5. Opcional: Esperar entrada para pausar
    printf("Presione Enter para continuar...");
    getchar();

    // char obj_file[256];
    // char exec_file[256];
    
    // snprintf(obj_file, sizeof(obj_file), "%s.o", asm_file);
    // snprintf(exec_file, sizeof(exec_file), "%s.out", asm_file);

    // // Ensamblar
    // char cmd[512];
    // snprintf(cmd, sizeof(cmd), "nasm -felf32 %s -o %s", asm_file, obj_file);
    // system(cmd);

    // // Linkear
    // snprintf(cmd, sizeof(cmd), "ld -m elf_i386 %s -o %s", obj_file, exec_file);
    // system(cmd);

    // // Ejecutar
    // snprintf(cmd, sizeof(cmd), "./%s", exec_file);
    // system(cmd);

    


    // NEW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
    // char cmd[512];
    // snprintf(cmd, sizeof(cmd), "nasm %s -o compiled.o", asm_file);
    // system(cmd);
    // snprintf(cmd, sizeof(cmd), "ld -m elf_i386 compiled.o -o compiled.out");
    // system(cmd);

    // snprintf(cmd, sizeof(cmd), "./compiled.out; echo \"\\nProgram exited with $?\"");
    // system(cmd);
    // system("./compiled.out");
}

const char* node_type_to_string(NodeType type) {
    static const char* names[] = {
    [ PROGRAM_NODE ] = "PROGRAM_NODE",
    [ STMT_NODE ] = "STMT_NODE",
    [ ASSIGN_NODE ] = "ASSIGN_NODE",
    [ READ_NODE ] = "READ_NODE",
    [ WRITE_NODE ] = "WRITE_NODE",
    [ EXPR_NODE ] = "EXPR_NODE",
    [ ID_NODE ] = "ID_NODE",
    [ VALUE_NODE ] = "VALUE_NODE"
    };
    return names[type];
}