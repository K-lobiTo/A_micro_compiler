#include <stdio.h>
#include <stdlib.h>
#include "../include/io_utils.h"

char* read_stdin_to_end(size_t* out_size) {
    size_t cap = 1024, len = 0;
    char* buf = malloc(cap);
    if (!buf) return NULL;

    while (1) {
        len += fread(buf + len, 1, cap - len, stdin);
        if (feof(stdin)) break;
        if (ferror(stdin)) goto error;

        if (len == cap) {
            cap *= 2;
            char* new_buf = realloc(buf, cap);
            if (!new_buf) goto error;
            buf = new_buf;
        }
    }

    buf = realloc(buf, len + 1);
    buf[len] = '\0';
    if (out_size) *out_size = len;
    return buf;

error:
    free(buf);
    return NULL;
}