#ifndef UTILITIES_H
#define UTILITIES_H
/*
 * Define a clean/safe abstraction around retrieving user input.
 * Provide a method to parse & print a supplied XML string.
 */

// Includes 
#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// Define custom return values.
#define VALID 0
#define EMPTY -1
#define OFLOW -2

// Types to simplify data management.
struct cstr {
	char *str_ptr;
	size_t len;
};


// Utility functions.
void create_cstr(struct cstr *string);
void destroy_cstr(struct cstr *string);

int get_line_safe(char *prompt, char *out_buffer, size_t size);
int get_timed_line(char *prompt, char *out_buffer, size_t size, int seconds);

char* get_xml_value(char *xml, char* key);

#endif
