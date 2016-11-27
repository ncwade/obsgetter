#include "cstring.h"
#include <ctype.h>

// Populate custom string structure. Just ties the size and pointer into a
// common structure.
void create_cstr(struct cstr *string) {
    string->str_ptr = malloc(1);
    string->str_ptr[0] = '\0';
    string->len = 0;
}

// Destroy our pretty structure.
void destroy_cstr(struct cstr *string) {
    free(string->str_ptr);
    string->len = -1;
}

// Quick way to convert a char string to uppercase.
void convert_to_upper(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
}
