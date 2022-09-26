/*
    MoonCLI 🌚

    A tiny client to demonstrate using libcurl to handle basic HTTP requests in C.
*/
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOONCLI_URI "https://shrewdly.herokuapp.com/"
#define MOONCLI_UA "mooncli (libcurl/7.85.0)"
#define MOONCLI_MAX_DESC_LEN 256
#define MOONCLI_MAX_NAME_LEN 256
#define MOONCLI_MAX_BUFFER 4096

static int line_ct;
static char json_buffer[MOONCLI_MAX_BUFFER];
static CURL *curl;

size_t data_callback(char *buffer, size_t item_size, size_t item_ct, void *ignoreme);
size_t header_callback(char *buffer, size_t item_size, size_t item_ct, void *ignoreme);
void print_http_status(CURL *curl);
void request_failure(CURLcode *res);
void json_pp(void);

int main(void) {

    curl = curl_easy_init();
    CURLcode res;
    line_ct = 1;

    curl_easy_setopt(curl, CURLOPT_URL, MOONCLI_URI);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, MOONCLI_UA);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, data_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);

    if (!curl) {
        fprintf(stderr, "[!] Failed to initialize curllib.");
        return EXIT_FAILURE;
    } else {

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            request_failure(&res);
        }
        curl_easy_cleanup(curl);

    }

    return EXIT_SUCCESS;
}

size_t data_callback(char *buffer, size_t item_size, size_t item_ct, void *ignoreme) {
    /*
        Callback for handling chunks received from server.
    */
    size_t byte_ct = item_size * item_ct;

    print_http_status(curl);
    /* DEBUG PRINTS */
    //printf("[+] Chunk received: (%zu bytes)\n", byte_ct);
    //printf("%3d: ", line_ct++);

    /* Print the received chunk one byte at a time. */
    for (char *p = buffer, *q = json_buffer; p < &buffer[byte_ct]; ++p) {
        if (*p != '\n') {
            //putchar(*p);
            if (q < &json_buffer[MOONCLI_MAX_BUFFER]) {
                *q = *p;
                q++;
            }
        } else {
            /* DEBUG PRINTS */
            //putchar(*p);
            //printf("%3d: ", line_ct);
            ++line_ct;
        }
    }

    //printf("\n\n");
    json_pp();
    return byte_ct;
}

size_t header_callback(char *buffer, size_t item_size, size_t item_ct, void *ignoreme) {
    /*
        Callback for handling HTTP header received from server.
    */
   size_t byte_ct = item_size * item_ct;

   return byte_ct;
}

void request_failure(CURLcode *res) {
    /*
        Use curl_easy_strerror to print a detailed error message.
        https://curl.se/libcurl/c/curl_easy_strerror.html
    */
    fprintf(stderr, "[!] %s", curl_easy_strerror(*res));
    exit(EXIT_FAILURE);
}

void print_http_status(CURL *curl) {
    /*
        DEBUG: Print HTTP Status
        TODO: Additional cases as needed based on what's defined by the API.
    */
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    printf("[+] Status: ");
    switch (http_code) {
        case 200:
            printf("200 OK\n");
            break;
    }
}

void json_pp(void) {
    struct json_object *parsed_json;
    struct json_object *json_data;
    struct json_object *json_metadata;
    struct json_object *json_message;
    struct json_object *json_description;
    struct json_object *json_name;

    struct Message {
        char description[MOONCLI_MAX_DESC_LEN];
        char name[MOONCLI_MAX_NAME_LEN];
    } message;

    parsed_json = json_tokener_parse(json_buffer);
    json_object_object_get_ex(parsed_json, "data", &json_data);
    json_object_object_get_ex(parsed_json, "meta", &json_metadata);
    json_object_object_get_ex(json_data, "message", &json_message);
    json_object_object_get_ex(json_message, "description", &json_description);
    json_object_object_get_ex(json_message, "name", &json_name);

    strncpy(message.description, json_object_get_string(json_description), MOONCLI_MAX_DESC_LEN);
    strncpy(message.name, json_object_get_string(json_name), MOONCLI_MAX_NAME_LEN);

    printf("Name: %s\n", message.name);
    printf("Description: %s\n", message.description);
}