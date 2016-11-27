#ifndef CSTRING_H
#define CSTRING_H
/*
 * Self-contained string representation.
 */

// Includes
#include <stdlib.h>
#include <string.h>

// Types to simplify data management.
struct cstr {
    char *str_ptr;
    size_t len;
};


// Type related.
void create_cstr(struct cstr *string);
void destroy_cstr(struct cstr *string);
void convert_to_upper(char* string);

#endif
