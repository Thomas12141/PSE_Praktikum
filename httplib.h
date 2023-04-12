//
// Created by Sebastian Schinzel on 25.03.20.
//

#ifndef ECHO_SERVER_HTTPLIB_H
#define ECHO_SERVER_HTTPLIB_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct string {
    size_t len;
    char* str;
};
typedef struct string string;


struct http_response_header {

    float http_version;
    int status_code;
    string reason_phrase;
    size_t content_length;
    string content_type;
};
typedef struct http_response_header http_response_header;

struct http_response {

    http_response_header header;
    string http_body;
};
typedef struct http_response http_response;



string* str_cat(string* dest, const char* src, size_t len);
string* _new_string();
void print_string(string* str);
string* cpy_str(const char* src, size_t len);
void free_str(string* str);
size_t get_length(string* str);
char* get_char_str(string* str);

#endif //ECHO_SERVER_HTTPLIB_H
