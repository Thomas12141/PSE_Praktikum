//
// Created by Sebastian Schinzel on 25.03.20.
//

#ifdef linux
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif

#ifndef ECHO_SERVER_HTTPLIB_H
#define ECHO_SERVER_HTTPLIB_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strlib.h"
#include "config.h"

struct http_request {
    string* method; // z. B. GET
    string* resource_path; // z. B. /index.html
    string* protocol; // HTTP Version z. B. HTTP/1.1
    string* hostname; //z. B. Host: localhost
};
typedef struct http_request http_request;

struct http_response_header {

    string* protocol; //HTTP Version z. B. HTTP/1.1
    string* status_code; // 3-stellige Ganzzahl zur kurzen Beschreibung des Zustandes
    string* reason_phrase; // Beschreibung des Zustandes in Textform
    unsigned long content_length;  // Leange des Inhalts
    string* content_type;   // Art des Inhalts
};
typedef struct http_response_header http_response_header;

struct http_response {
    http_response_header* header;
    string* http_body;
};
typedef struct http_response http_response;

http_request* getRequestStruct(string* str);
char* getFilePath(http_request* request);
string* getResponseString(http_response* response);
string* getFiletype (char* resource_path, int len);
void freeRequestStruct(http_request* req);
#endif //ECHO_SERVER_HTTPLIB_H
