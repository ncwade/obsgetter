#ifndef HTTP_H
#define HTTP_H
/*
 * Wrappers for HTTP commands.
 */

// Includes
#include "cstring.h"

// Main Interfaces
void http_file_fetch(char *host, char *file_name, struct cstr *string);

#endif
