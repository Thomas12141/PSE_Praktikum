//
// Created by Matteo Illing on 27.03.23.
//

#include "httplib.h"

string* sanitizeRequestedResource(string* resource_path) {
    if(resource_path->str[resource_path->len - 1] == '/') {
        resource_path = str_cat(resource_path, "index.html", 10);
    }

    return resource_path;
}

http_request* getRequestStruct(string* request_string){
    http_request* request = calloc(sizeof(http_request), 1);

    if(request == NULL) {
        exit(2);
    }

    request->method = _new_string();
    request->resource_path = _new_string();
    request->protocol = _new_string();

    int endpositionen[3];
    int argumentCount = 0;

    for (int i = 0; i < request_string->len; i++) {
        if (argumentCount == 3) {
            break;
        }

        if (request_string->str[i+1] == ' ' || request_string->str[i+1] == '\r') {
            endpositionen[argumentCount] = i;
            argumentCount++;
        }
    }

    size_t method_size = endpositionen[0] + 1;
    size_t resource_size = endpositionen[1] - method_size;
    size_t protocol_size = endpositionen[2] - resource_size - method_size - 1;

    request->method->str = realloc(request->method->str, method_size);
    request->resource_path->str = realloc(request->resource_path->str, resource_size);
    request->protocol->str = realloc(request->protocol->str, protocol_size);

    memcpy(request->method->str, request_string->str, method_size);
    request->method->len = method_size;
    memcpy(request->resource_path->str, request_string->str + endpositionen[0] + 2, resource_size);
    request->resource_path->len = resource_size;
    memcpy(request->protocol->str, request_string->str + endpositionen[1] + 2, protocol_size);
    request->protocol->len = protocol_size;

    return request;
}

char* getFilePath(http_request* request) {
    char pathBuffer[PATH_MAX];

    string* docroot = cpy_str(DOCROOT, strlen(DOCROOT));
    string* file_path = str_cat(docroot, request->resource_path->str, request->resource_path->len);

    char* filepathPointer = calloc(file_path->len + 1, 1);
    memcpy(filepathPointer, file_path->str, file_path->len);

    realpath(filepathPointer, pathBuffer);

    unsigned long pathLength = strlen(pathBuffer);
    char* shortenedPath = calloc(strlen(pathBuffer) + 1, 1);
    memcpy(shortenedPath, pathBuffer, pathLength);

    return shortenedPath;
}

string* getResponseString(http_response* response) {
    //maximum value of ulong is 4294967295 -> has 10 digits
    char* contentSizeBuffer = calloc(10, 1);
    snprintf(contentSizeBuffer, 10, "%ld", response->header->content_length);

    string* responseStr = cpy_str(response->header->protocol->str, response->header->protocol->len);
    responseStr = str_cat(responseStr, " ", 1);
    responseStr = str_cat(responseStr, response->header->status_code->str, response->header->status_code->len);
    responseStr = str_cat(responseStr, " ", 1);
    responseStr = str_cat(responseStr, response->header->reason_phrase->str, response->header->reason_phrase->len);
    if(response->header->content_type != NULL) {
        responseStr = str_cat(responseStr, "\r\n", 2);
        responseStr = str_cat(responseStr, "Content-Type: ", 14);
        responseStr = str_cat(responseStr, response->header->content_type->str, response->header->content_type->len);
    }
    responseStr = str_cat(responseStr, "\r\n", 2);
    responseStr = str_cat(responseStr, "Content-Length: ", 16);
    responseStr = str_cat(responseStr, contentSizeBuffer, strlen(contentSizeBuffer));
    responseStr = str_cat(responseStr, "\r\n\r\n", 4);
    responseStr = str_cat(responseStr, response->http_body->str, response->http_body->len);

    return responseStr;
}
