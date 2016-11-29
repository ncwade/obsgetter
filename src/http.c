#include "http.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h> 

// Take data from a pointer and store it in supplied cstring.
size_t store_data(void *data, size_t size, struct cstr *string) {
    // Figure out how much more room we need.
    size_t new_len = string->len + size;
    // Use the new length to reallocate our pointer.
    string->str_ptr = realloc(string->str_ptr, new_len+1);
    // Fill the buffer with nulls.
    memset(string->str_ptr+string->len, '\0', size+1);
    // Copy the data from the curl to our user data.
    memcpy(string->str_ptr+string->len, data, size);
    // Update the length.
    string->len = new_len;
    // Tell caller how much data we wrote.
    return size;
}

void remove_http_header(struct cstr *string) {
    // HTTP headers are terminated with two DOS line endings.
    char *header_end = strstr(string->str_ptr, "\r\n\r\n");

    // See if we found it or not.
    if (header_end != NULL) {
        size_t index = header_end - string->str_ptr + 4;
        // Determine how much data we need in the new string.
        size_t new_len = string->len - index;
        // Allocate new string.
        char *new_str = malloc(new_len + 1);
        memset(new_str, '\0', new_len + 1);
        // Copy just the file fropm the old string location.
        memcpy(new_str, string->str_ptr+index, new_len);
        // Remove old string.
        free(string->str_ptr);
        // Replace with the new string we just allocated.
        string->str_ptr = new_str;
        string->len = new_len;
    }
}

// Wrap the retrival of files over HTTP. This should work with any HTTP server
//that doesn't require streaming support.
void http_file_fetch(char *host, char *file_name, struct cstr *string) {
    // Allocate a socket and ensure it's valid.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd > 0){
        // Convert a hostname to server structure and ensure it's valid.
        struct hostent *server  = gethostbyname(host);
        if (server != NULL) {
            // Create/format structures needed by the socket connection.
            struct sockaddr_in serveraddr;
            memset(&serveraddr, '\0', sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;
            memcpy((char *)&serveraddr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
            serveraddr.sin_port = htons(80);

            // Establish a connection to the server using created structures.
            if (connect(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == 0) {
                // Build and format the HTTP string.
                char input[104 + strlen(file_name)];
                snprintf(input, sizeof(input), "GET %s %s\r\n%s\r\n%s\r\n%s\r\n%s\r\n\r\n",
                                                file_name,
                                                "HTTP/1.0",
                                                "Host: w1.weather.gov",
                                                "Connection: close",
                                                "User-Agent: obsgetter/1.0",
                                                "Accept: */*");

                // Send HTTP string to remote server.
                // If the transmission is successful, start reading return data.
                if (write(sockfd, input, strlen(input)) > 0) {
                    int rc;
                    do {
                        char xml[1024] = {0};
                        rc = read(sockfd, xml, sizeof(xml));

                        // If data was received, save it to our cstring.
                        if (rc > 0) {
                            store_data(xml, rc, string);
                        }
                        // Read until the read call returns 0(EOF) or < 0(error).
                    } while(rc > 0); 

                    // Cleanup the file we retrieved.
                    remove_http_header(string);
                }
            }
        }

        // Socket was opened so close it.
        close(sockfd);
    }
}