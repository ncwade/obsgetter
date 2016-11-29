#include "safe_io.h"
#include "xml_utils.h"
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

// Take an XML document and parse the required weather data from it. The data
// is displayed per the definition within weather.cfg. We expect this to be in
// the format of title=key. Comment support is very primitive
void print_weather(char *xml) {
    FILE *fp = fopen("weather.cfg" , "r");
    if(fp) {
        char buf[102] = {0};
        printf("Requested Weather Report\n");
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            // Basic comment support.
            if (buf[0] != '#') {
                char title[50];
                char key[50];
                sscanf(buf, "%50[^'=']=%50s", title, key);
                printf("%s: ", title);
                print_xml_value(xml, key);
                printf("\n");
            }
        }
    } else {
        printf("Missing weather.cfg. Please recreate.\n");
    }
}

// Entry point.
int main() {
    char run_again[3] = {0};
    // Register our signal handler. We want to use the alarm
    // functionality, so we need to register for SIGALRM.
    if (signal(SIGALRM, signal_cb) == SIG_ERR) {
        printf("Could not register signal handler.\n");
        return -5;
    }

    // Pull the XML collection of stations.
    struct cstr stations;
    create_cstr(&stations);
    http_file_fetch("w1.weather.gov", "/xml/current_obs/index.xml", &stations);

    // Nest in loop so we can get as many reports as the user would like.
    do {
        int rc;
        char state[4] = {0};
        char id_buff[6] = {0};

        // Get the two character station ID from user.
        rc = get_timed_line("Enter two-digit state code> ", state, sizeof(state), 60);
        // If we have valid data we can proceed.
        if (rc == VALID) {
            convert_to_upper(state);
            // Print the stations that match the state code.
            pretty_print_stations(stations.str_ptr, state);

            rc = get_timed_line("Enter station ID> ", id_buff, sizeof(id_buff), 60);
            if (rc == VALID) {
                convert_to_upper(id_buff);
                // Since we checked verified the size of the ID buffer we know 256 bytes is enough.
                char file_path[256] = {0};
                // Format the file_path.
                snprintf(file_path, sizeof(file_path), "/xml/current_obs/%s.xml", id_buff);
                // Use the custom string type used by our HTTP wrapper.
                struct cstr xml;
                create_cstr(&xml);
                // Pull XML from server.
                http_file_fetch("w1.weather.gov", file_path, &xml);
                // Pass XML to formatting function.
                print_weather(xml.str_ptr);
                // Cleanup.
                destroy_cstr(&xml);
            } else {
                printf("Invalid station selected.\n");
            }
        } else {
            printf("Invalid state selected.\n");
        }

        // Prompt the user for exit or continue.
        rc = get_timed_line("Would you like to submit another weather request[Y/n]> ", run_again, sizeof(run_again), 60);
        convert_to_upper(run_again);
    } while(strncmp(run_again, "Y", strlen("Y")) == 0);

    // Delete our station XML file.
    destroy_cstr(&stations);
    return 0;
}
