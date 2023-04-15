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

struct http_request {
    string* method; // z. B. GET
    string* resource_path; // z. B. /index.html
    string* protocol; // HTTP Version z. B. HTTP/1.1
    string* hostname; //z. B. Host: localhost
};
typedef struct http_request http_request;

struct http_response_header {

    string* protocol; //HTTP Version z. B. HTTP/1.1
    int status_code; // 3-stellige Ganzzahl zur kurzen Beschreibung des Zustandes
    string* reason_phrase; // Beschreibung des Zustandes in Textform
    size_t content_length;  // Leange des Inhalts
    string* content_type;   // Art des Inhalts
}
;typedef struct http_response_header http_response_header;

struct http_response {
    http_response_header header;
    string* http_body;
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
