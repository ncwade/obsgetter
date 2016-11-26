#include "http.h"
#include <string.h>

// Custom data handler. The footprint for this can be found at
// https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t store_data(void *data, size_t size, size_t nmemb, struct cstr *string) {
    // Figure out how much more room we need.
    size_t new_len = string->len + size*nmemb;
    // Use the new length to reallocate our pointer.
    string->str_ptr = realloc(string->str_ptr, new_len+1);
    // Copy the data from the curl to our user data.
    memcpy(string->str_ptr+string->len, data, size*nmemb);
    // Null terminate opur string with the added data.
    string->str_ptr[new_len] = '\0';
    // Update the length.
    string->len = new_len;

    // Tell curl we wrote all the data.
    return size*nmemb;
}

// A simple wrapper around the libcurl library. Allows us to provie a url and
// structure to store the content in.
void http_get(char *url, struct cstr *string) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Have curl pass the data to our custom handler to be saved in memory.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, store_data);
        // Give curl the ptr to our data store.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, string);
        // Apparently if you don't set a user agent you get permission denied.
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "obsgetter/1.0");
        // Kick off the operation.
        res = curl_easy_perform(curl);
        // Cleanup the resources that were allocated.
        curl_easy_cleanup(curl);
    }
}