#ifndef UTILITIES_H
#define UTILITIES_H
/*
 * Define a clean/safe abstraction around retrieving user input.
 */

// Includes
#include <stdlib.h>

// Define custom return values.
#define VALID 0
#define EMPTY -1
#define OFLOW -2

// Input related.
int get_line_safe(char *prompt, char *out_buffer, size_t size);
int get_timed_line(char *prompt, char *out_buffer, size_t size, int seconds);

#endif
