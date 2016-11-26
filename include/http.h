#ifndef HTTP_H
#define HTTP_H
/*
 * Wrappers for HTTP commands.
 */

// Includes
#include <curl/curl.h>
#include "utilities.h"

// Main Interfaces
void http_get(char *url, struct cstr *string);

#endif
