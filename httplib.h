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
#include "http_status.h"

/**
 * @struct http_request
 * @brief Diese Struktur enthält alle nötigen Eigenschaften einer http_request.
 */
struct http_request {
    string* method; /**< Die HTTP-Methode.*/
    string* resource_path; /**< Der Dateipfad der angeforderten Ressource.*/
    string* protocol; /**< Die HTTP-Version.*/
    string* hostname; /**< Hier wird angegeben, ob auf die interne oder externe Seite zugegriffen werden soll.*/
    string* credentials; /**< Wenn es keine credentials gibt wird der Wert auf NULL gesetzt, wenn ja, dann wird dort als string username und passwort gesetzt
 * mit ":" dazwischen*/
};
typedef struct http_request http_request;

/**
 * @struct http_response_header
 * @brief Diese Struktur enthält alle nötigen Eigenschaften eines http_response_header.
 */
struct http_response_header {
    string* protocol; /**< Die HTTP-Version.*/
    string* status_code; /**< 3-stellige Ganzzahl zur kurzen Beschreibung des Zustandes.*/
    string* reason_phrase; /**< Beschreibung des Zustandes in Textform.*/
    unsigned long content_length;  /**< Länge des Inhalts.*/
    string* content_type;   /**< Art des Inhalts.*/
    int isAuthenticationRequired; /**< Wahrheitswert, ob eine Authentifikation durchgeführt werden muss.*/
};
typedef struct http_response_header http_response_header;

/**
 * @struct http_response
 * @brief Diese Struktur enthält alle nötigen Eigenschaften einer http_response.
 */
struct http_response {
    http_response_header* header; /**< Der Response-Header*/
    string* http_body; /**< Der Response-Body.*/
};
typedef struct http_response http_response;

http_request* getRequestStruct(string* str);
string* getResponseString(http_response* response);
void freeRequestStruct(http_request* req);
string* getContentType(string* fileType);
http_response* getShortResponse(char* statusCode, char* message);
void sanitizeRequestedResource(http_request* request);
string* getCredentialsString(string* request_string);
#endif //ECHO_SERVER_HTTPLIB_H
