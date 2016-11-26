#include "utilities.h"
#include <string.h>
#include <unistd.h>
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

// Use the alarm system call to ensure this prompt doesn't block forever.
int get_timed_line(char *prompt, char *out_buffer, size_t size, int seconds) {
    // Set alarm
    alarm(seconds);
    // Get input
    int rc = get_line_safe(prompt, out_buffer, size);
    // Reset alarm
    alarm(0);
    return rc;
}

// Ensure that when data is retrieved from the console that;
//  - It fits in the supplied buffer.
//  - A prompt is printed if needed.
//  - That the buffer is properly terminated when finished.
//  - That STDIN is flushed so we don't corrupt the next input.
int get_line_safe(char *prompt, char *out_buffer, size_t size) {
    int byte;

    // Print the prompt if provided.
    if (prompt != NULL) {
        printf ("%s", prompt);
        fflush (stdout);
    }

    // Read all input supplied by the user.
    if(fgets(out_buffer, size, stdin) == NULL) {
        return EMPTY;
    }

    // Grab the terminating character.
    byte = out_buffer[strlen(out_buffer)-1];

    // Check if we get the terminating newline in our buffer.
    if (byte != '\n' && byte != EOF) {
        // Clear our any data remaining in the buffer.
        do {
            byte = getchar();
        } while (byte != '\n' && byte != EOF);

        // Now inform the user their buffer wasn't big enough.
        return OFLOW;
    }

    // Null terminate in place of the newline.
    out_buffer[strlen(out_buffer)-1] = '\0';
    return VALID;
}

// Used by get_xml_value to recursively search for your key.
char* _get_xml_value(xmlNode *a_node, char* key) {
    char *value = NULL;

    // We need to search every node.
    for (xmlNode *cur_node = a_node; cur_node; cur_node = cur_node->next) {
        // Check if this node is the node we are looking for.
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)cur_node->name, key) == 0) {
                // Found it!
                return (char*)xmlNodeGetContent(cur_node);
            }
        }

        // Check if any of this nodes children are the ones we are looking for.
        value = _get_xml_value(cur_node->children, key);
        if (value != NULL) {
            // Found it!
            return value;
        }
    }

    // Return null if not found. The final destination needs to handle this.
    return value;
}

// Search the given XML string for the provided key.
char* get_xml_value(char *xml, char* key) {
    // Read the XML data from RAM.
    xmlDoc *doc = xmlReadMemory(xml, strlen(xml), "file.xml", NULL, XML_PARSE_NOERROR | XML_PARSE_NOWARNING );
    // Our searching function requires a node to start with, grab root.
    xmlNode *root = xmlDocGetRootElement(doc);
    // Find the key!
    char* value = _get_xml_value(root, key);
    // Destroy the objects we allocated.
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return value;
}

// Used by print_xml_values to find all stations that match a state.
// TODO: Can we clean this up?
void _print_station_info(xmlNode *a_node, char* state) {
    // Keep track of how many columns we have printed
    int count = 0;

    // We need to search every node.
    for (xmlNode *cur_node = a_node; cur_node; cur_node = cur_node->next) {
        // Make sure it's a node.
        if (cur_node->type == XML_ELEMENT_NODE) {
            // Make sure this is a station.
            if (strcmp((char*)cur_node->name, "station") == 0) {
                char *state_field = NULL;
                char *id = NULL;
                // Loop through each field in the station.
                for (xmlNode *station = cur_node->children; station; station = station->next) {
                    // Find the two fields we are interested in; ID & state.
                    if (strcmp((char*)station->name, "state") == 0) {
                        state_field = (char*)xmlNodeGetContent(station);
                    } else if (strcmp((char*)station->name, "station_id") == 0) {
                        id = (char*)xmlNodeGetContent(station);
                    }
                }

                // If this station is in the right state, pretty print it.
                if (strcmp(state_field, state) == 0) {
                    printf("%s (%s)\t", id, state_field);
                    count++;
                    // Five is an arbitrary column value.
                    if (count % 5 == 0) {
                        printf("\n");
                    }
                }
            }
        }
    }
}

// Print all of the weather stations associated with a given state code.
void pretty_print_stations(char *xml, char* state) {
    // Read the XML data from RAM.
    xmlDoc *doc = xmlReadMemory(xml, strlen(xml), "file.xml", NULL, XML_PARSE_NOERROR | XML_PARSE_NOWARNING );
    // Our searching function requires a node to start with, grab root.
    xmlNode *root = xmlDocGetRootElement(doc);
    // Find the key!
    _print_station_info(root->children, state);
    // Ensure any following data is on a clean line.
    printf("\n");
    // Destroy the objects we allocated.
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
