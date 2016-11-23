#include "utilities.h"
#include "http.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Define a signal handler we can use to exit the program.
void signal_cb(int sig) {
    if (sig == SIGALRM) {
        printf("\nTimed out waiting for user input. Exiting.\n");
    }
    exit(sig);
}

// Take an XML document and parse the required weather data from it.
void print_weather(char *xml) {
    printf("Requested Weather Report\n");
    printf("Station: %s\n", get_xml_value(xml, "station_id"));
    printf("Location: %s\n", get_xml_value(xml, "location"));
    printf("Station: %s\n", get_xml_value(xml, "weather"));
    printf("Station: %s\n", get_xml_value(xml, "wind_string"));
    printf("Station: %s\n", get_xml_value(xml, "temperature_string"));
    printf("Station: %s\n", get_xml_value(xml, "relative_humidity"));
}

int main() {
    int rc;
    char state_buff[40];
    char id_buff[10];
    char run_again[5];

    // Register our signal handler. We want to use the alarm
    // functionality, so we need to register for SIGALRM.
    if (signal(SIGALRM, signal_cb) == SIG_ERR) {
        printf("Could not register signal handler.\n");
        return -5;
    }

    // Nest in loop so we can get as many reports as the user would like.
    do {
        rc = get_timed_line("Enter State or Territory> ", state_buff, sizeof(state_buff), 60);

        // If we have valid data we can proceed.
        if (rc == VALID) {
            rc = get_timed_line("Enter station ID> ", id_buff, sizeof(id_buff), 60);
            if (rc == VALID) {
                // Since we checked verified the size of the ID buffer we know 256 bytes is enough.
                char url[256];
                // Format the URL.
                snprintf(url, sizeof(url), "http://w1.weather.gov/xml/current_obs/%s.xml", id_buff);
                // Use the custom string type used by our HTTP wrapper.
                struct cstr xml;
                create_cstr(&xml);
                // Pull XML from server.
                http_get(url, &xml);
                // Pass XML to formatting function.
                print_weather(xml.str_ptr);
                // Cleanup.
                destroy_cstr(&xml);
            } else {
                printf("Invalid station selected, exiting.\n");
            }
        } else {
            printf("Invalid state selected, exiting.\n");
        }

        // Prompt the user for exit or continue.
        rc = get_timed_line("Would you like to submit another weather report[Y/n]> ", run_again, sizeof(run_again), 60);
    } while(strcmp(run_again, "Y") == 0);

    return 0;
}