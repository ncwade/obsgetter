#include "xml_utils.h"
#include <string.h>

// Used by print_xml_value to recursively search for your key.
void _print_xml_value(xmlNode *a_node, char* key) {
    // We need to search every node.
    for (xmlNode *cur_node = a_node; cur_node; cur_node = cur_node->next) {
        // Check if this node is the node we are looking for.
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)cur_node->name, key) == 0) {
                // Found it!
                printf("%s",xmlNodeGetContent(cur_node));
            }
        }

        // Check if any of this nodes children are the ones we are looking for.
        _print_xml_value(cur_node->children, key);
    }
}

// Search the given XML string for the provided key.
void print_xml_value(char *xml, char* key) {
    // Read the XML data from RAM.
    xmlDoc *doc = xmlReadMemory(xml, strlen(xml), "file.xml", NULL, XML_PARSE_NOERROR | XML_PARSE_NOWARNING );
    // Our searching function requires a node to start with, grab root.
    xmlNode *root = xmlDocGetRootElement(doc);
    // Find the key!
    _print_xml_value(root, key);
    // Destroy the objects we allocated.
    xmlFreeDoc(doc);
    xmlCleanupParser();
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
