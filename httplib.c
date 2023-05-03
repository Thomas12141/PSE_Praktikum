//
// Created by Matteo Illing on 27.03.23.
//

#include "httplib.h"

/**
 * Hängt die Datei index.html an den übergebenen Dateipfad an, wenn er auf ein "/" endet.
 * Wenn der Dateipfad nicht auf "/" endet, wird er ohne Änderung zurückgegeben.
 *
 * @param resource_path Dateipfad, an den index.html angehängt wird.
 * @return Dateipfad mit index.html.
 */
string* sanitizeRequestedResource(string* resource_path) {
    resource_path = decodeString(resource_path);

    if(resource_path->str[resource_path->len - 1] == '/') {
        resource_path = str_cat(resource_path, "index.html", 10);
    }

    return resource_path;
}

/**
 * Übersetzt einen request_string der Form String* in ein request der Form http_request.
 *
 * @param request_string Der zu übersetzende String*.
 * @return Ein http_request struct mit den Attributen method, resource_path und protocol.
 */
http_request* getRequestStruct(string* request_string){
    http_request* request = calloc(sizeof(http_request), 1);

    if(request == NULL) {
        exit(2);
    }

    int endpositionen[3];
    int argumentCount = 0;

    for (int i = 0; i < request_string->len; i++) {
        if (request_string->str[i+1] == ' ' || request_string->str[i+1] == '\r') {
            endpositionen[argumentCount] = i;
            argumentCount++;
        }

        if (argumentCount == 3 || request_string->str[i+1] == '\r') {
            break;
        }
    }

    if (argumentCount != 3) {
        return NULL;
    }

    size_t method_size = endpositionen[0] + 1;
    size_t resource_size = endpositionen[1] - method_size;
    size_t protocol_size = endpositionen[2] - resource_size - method_size - 1;

    request->method = cpy_str(request_string->str, method_size);
    string* requestedResource = sanitizeRequestedResource(cpy_str(request_string->str + endpositionen[0] + 2, resource_size));
    request->resource_path = requestedResource;
    request->protocol = cpy_str(request_string->str + endpositionen[1] + 2, protocol_size);

    return request;
}

/**
 * Konstruiert den Dateipfad für das übergebene Objekt des Typs http_request*.
 *
 * @param request Das http_request*-Objekt, für das der Dateipfad konstruiert werden soll.
 * @return Der Dateipfad als char*.
 */
char* getFilePath(http_request* request) {
    string* docroot = cpy_str(DOCROOT, strlen(DOCROOT));
    string* file_path = str_cat(docroot, request->resource_path->str, request->resource_path->len);

    char* filepathPointer = calloc(file_path->len + 1, 1);
    memcpy(filepathPointer, file_path->str, file_path->len);

    char *pathBuffer = realpath(filepathPointer, NULL);
    if(pathBuffer == NULL) {
        return NULL;
    }

    unsigned long pathLength = strlen(pathBuffer);
    char* shortenedPath = calloc(strlen(pathBuffer) + 1, 1);
    memcpy(shortenedPath, pathBuffer, pathLength);

    free(pathBuffer);

    return shortenedPath;
}

/**
 * Gibt die http-response als String* zurück.
 *
 * @param response http-response der Form struct http_response.
 * @return http-response der Form String*.
 */
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

    free(contentSizeBuffer);

    return responseStr;
}

string* getFiletype (char* resource_path, int len) {

    string* content_type = calloc(sizeof(string), 1);

    content_type->str = calloc(1, len);
    if(content_type->str == NULL) {
        exit(3);
    }
    memcpy(content_type->str, resource_path, len);
    content_type->len = len;

    int dot_position;

    for (int i = len; i >= 0; i--){
       if (resource_path[i] == '.'){
           dot_position = i;
       }
    }

    content_type = cpy_str(resource_path+dot_position+1, len - dot_position);

    return content_type;
}

string* getContentType(string* fileType){

    string* contentType = calloc(sizeof(string), 1);
    if(contentType == NULL) {
        exit(3);
    }

    char *contentMap[10][1][2] = {
            {{"acc", "audio/acc"} },
            {{"txt","text/txt"} },
            {{"png", "images/png"} }
    };

    for (int x = 0; x < 3; x++) {
        for (int y = 0; x < 3; x++) {
            int type_length = get_char_length(contentMap[x][0][0]);
            if (char_cmp(fileType->str, contentMap[x][0][0], fileType->len, type_length)) {
                int contentType_length = get_char_length(contentMap[x][y][1]);
                contentType = cpy_str(contentMap[x][0][1],contentType_length);
            }
        }
    }


    return contentType;
}
