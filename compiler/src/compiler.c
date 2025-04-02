
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io_utils.h"
#include "../include/scanner.h"


int main() {
    size_t size;
    char* source = read_stdin_to_end(&size);
    if (!source) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    

    printf("%.*s\nRead %zu bytes\n", size, source, (int)size); // for testing [it worked]
    Scanner sc;
    scanner_init(&sc, source);
    Token actToken = {TOKEN_ERROR, {0}};

    while (actToken.type != TOKEN_EOF){ // for testing [works as expected]
        actToken = next_token(&sc);
        printf("TokenType %zu: Lexeme: %s\n", actToken.type, actToken.lexeme);
    }
    
    free(source);
    return 0;


    // printf("\nbits %zu value\n", size,  ((2) < 8 ? ((1 << (2)) << 8) : ((1 << (2)) >> 8))); 
}