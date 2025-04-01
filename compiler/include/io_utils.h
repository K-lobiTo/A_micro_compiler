#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stddef.h>

// Reads stdin until EOF, returns dynamically allocated string
char* read_stdin_to_end(size_t* out_size); 

#endif