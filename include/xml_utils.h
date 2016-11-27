#ifndef XML_UTILS_H
#define XML_UTILS_H
/*
 * Provide a method to print the value of a supplied key from the provided XML.
 * Provide a method to print the all the stations for given two digit state code.
 */

// Includes
#include <libxml/parser.h>
#include <libxml/tree.h>

void print_xml_value(char *xml, char* key);
void pretty_print_stations(char *xml, char* state);

#endif
