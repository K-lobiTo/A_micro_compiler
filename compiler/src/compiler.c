
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io_utils.h"


int main() {
    size_t size;
    char* source = read_stdin_to_end(&size);
    if (!source) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    printf("%.*s\nRead %zu bytes\n", size, source, (int)size); // for testing [it worked]
    free(source);
    return 0;
}