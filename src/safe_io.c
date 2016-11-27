#include "safe_io.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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
