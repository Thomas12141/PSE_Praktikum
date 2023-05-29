//
// Created by Matteo Illing on 27.03.23.
//

#include "httplib.h"
#include "filelib.h"

/**
 * Hängt die Datei index.html an den übergebenen Dateipfad an, wenn er auf ein "/" endet.
 * Wenn der Dateipfad nicht auf "/" endet, wird er ohne Änderung zurückgegeben.
 *
 * @author Matteo Illing
 * @param resource_path Dateipfad, an den index.html angehängt wird.
 * @return Dateipfad mit index.html.
 */
void sanitizeRequestedResource(http_request* request) {
    request->resource_path = decodeString(request->resource_path);

    string* absoluteResourcePath = getDocrootpath(request->hostname);
    char* filepathBuffer = calloc(PATH_MAX, 1);
    absoluteResourcePath = str_cat(absoluteResourcePath, request->resource_path->str, request->resource_path->len);
    char* filepath = calloc(absoluteResourcePath->len+1, 1);
    memcpy(filepath, absoluteResourcePath->str, absoluteResourcePath->len);
    free_str(absoluteResourcePath);
    realpath(filepath, filepathBuffer);

    string* fileTest = readFile(filepathBuffer);
    if(fileTest == NULL || (fileTest->len > 0 && fileTest->str[0] == '\0')) {
        request->resource_path = str_cat(request->resource_path, "/index.html", 11);
    }

    if(fileTest)
        free_str(fileTest);
    free(filepathBuffer);
    free(filepath);
}

/**
 * Schpeichert die Credentials, wenn die gibt, wenn nicht wird auf Null gesetzt
 *
 * @author Thomas Fidorin
 * @param request_string der request vom Server
 * @return Die Credentials oder NULL, wenn den nicht gibt
 */
string* getCredentialsString(string* request_string){
    string* authorizationString = cpy_str("Authorization: Basic ", strlen("Authorization: Basic "));
    for(int i = 0; i< request_string->len; i++){
        int j;
        for (j = 0; j < authorizationString->len; ++j) {
            if(authorizationString->str[j]!=request_string->str[i]){
                i-=j;
                break;
            } else{i++;}
        }
        if(j==authorizationString->len){
            int count=0;
            while(request_string->str[i+count] != '\r' && request_string->str[i+count] != '\n') {
                count++;
            }
            free_str(authorizationString);
            string* credentials = cpy_str(&request_string->str[i], count);
            return credentials;
        }
    }
    free_str(authorizationString);
    return NULL;
}

/**
 * Übersetzt einen request_string der Form String* in ein request der Form http_request struct.
 *
 * @author Jeremy Beltran
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
        if(i+1 < request_string->len) {
            if (request_string->str[i+1] == ' ' || request_string->str[i+1] == '\r') {
                endpositionen[argumentCount] = i;
                argumentCount++;
            }

            if (argumentCount == 3 || request_string->str[i+1] == '\r' || request_string->str[i+1] == '\n') {
                break;
            }
        }
    }

    if (argumentCount != 3) {
        free(request);
        return NULL;
    }

    size_t method_size = endpositionen[0] + 1;
    size_t resource_size = endpositionen[1] - method_size;
    size_t protocol_size = endpositionen[2] - resource_size - method_size - 1;

    if(method_size == 0 || resource_size == 0 || protocol_size == 0) {
        free(request);
        return NULL;
    }

    request->method = cpy_str(request_string->str, method_size);
    request->resource_path = cpy_str(request_string->str + endpositionen[0] + 2, resource_size);
    request->protocol = cpy_str(request_string->str + endpositionen[1] + 2, protocol_size);

    string* hostnameString = cpy_str("host:", 5);
    int hostnamePositions[2];
    for(int i = endpositionen[2]; i < request_string->len; i++) {
        if(i + 4 < request_string->len) {
            string* paramStr = str_lower(cpy_str(request_string->str + i, 5));
            if(str_cmp(hostnameString, paramStr)) {
                i+=hostnameString->len;

                while(request_string->str[i] == ' ') {
                    i++;
                }
                hostnamePositions[0] = i;
                while(request_string->str[i] != '\r' && request_string->str[i] != ':' && request_string->str[i] != ' ') {
                    i++;
                }
                hostnamePositions[1] = i;
                request->hostname = cpy_str(request_string->str + hostnamePositions[0], hostnamePositions[1] - hostnamePositions[0]);
                free_str(paramStr);
                break;
            }
            free_str(paramStr);
        }
    }

    if(request->hostname == NULL) {
        request->hostname = cpy_str("default", 7);
    }

    free_str(hostnameString);

    request->credentials=getCredentialsString(request_string);
    return request;
}


/**
 * Gibt die http-response als String* zurück.
 *
 * @author Matteo Illing
 * @param response http-response der Form struct http_response.
 * @return http-response der Form String*.
 */
string* getResponseString(http_response* response) {
    //maximum value of ulong is 4294967295 -> has 10 digits
    char* contentSizeBuffer = calloc(10, 1);
    if (contentSizeBuffer == NULL){
        exit(3);
    }
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
    if(response->header->isAuthenticationRequired) {
        responseStr = str_cat(responseStr, "\r\n", 2);
        responseStr = str_cat(responseStr, "WWW-Authenticate: ", 18);
    }
    responseStr = str_cat(responseStr, "\r\n\r\n", 4);
    responseStr = str_cat(responseStr, response->http_body->str, response->http_body->len);

    free(contentSizeBuffer);

    free_str(response->http_body);
    free_str(response->header->reason_phrase);
    free_str(response->header->status_code);
    free_str(response->header->protocol);
    free_str(response->header->content_type);
    free(response->header);
    free(response);

    return responseStr;
}

/**
 * Gibt den Speicher eines http_request struct frei.
 *
 * @author Matteo Illing
 * @param req Der freizugebende http_request struct.
 */
void freeRequestStruct(http_request* req) {
    free_str(req->resource_path);
    free_str(req->protocol);
    free_str(req->method);
    if(req->hostname != NULL)
        free_str(req->hostname);
    if(req->credentials != NULL)
        free_str(req->credentials);
    free(req);
}

/**
 * Ermittelt den contentType der Datei auf Basis des fileTypes (Dateiendung, siehe getFileType()).
 *
 * @author Simon Lammers
 * @param fileType Die Dateiendung ohne Punkt als string*.
 * @return Den contentType als string*.
 */
string* getContentType(string* fileType){
    char* filetypeArray[13] = {"acc", "txt", "png", "css", "doc", "html",
                           "jpeg", "jpg", "mp3", "mp4", "mpeg", "pdf", "js"};

    char* contentTypeArray[13] = {"audio/acc", "text/plain", "image/png", "text/css",
                              "application/msword", "text/html", "image/jepg", "image/jpg",
                              "audio/mpeg", "video/mp4", "video/mpeg", "application/pdf", "text/javascript"};

    for (int x = 0; x < 13; x++) {
        int type_length = strlen(filetypeArray[x]);
            if (char_cmp(fileType->str, filetypeArray[x], fileType->len, type_length)) {
                int contentType_length = strlen(contentTypeArray[x]);
                free_str(fileType);
                return cpy_str(contentTypeArray[x],contentType_length);
            }
    }

    free_str(fileType);
    return _new_string();
}

/**
 * Konstruiert die Statusmeldungen in Form eines http_response structs.
 *
 * @author Matteo Illing
 * @param statusCode Der zu verwendende Statuscode.
 * @param message Die dazugehörige Status-message.
 * @return Der http_response struct.
 */
http_response* getShortResponse(char* statusCode, char* message) {
    http_response* response = calloc(1, sizeof(http_response));
    http_response_header* header = calloc(1, sizeof(http_response_header));
    response->header = header;
    header->content_type = cpy_str("text/plain", 10);
    header->protocol = cpy_str("HTTP/1.1", 8);

    header->status_code = cpy_str(statusCode, 3);
    header->reason_phrase = cpy_str(message, strlen(message));
    response->http_body = cpy_str(statusCode, strlen(statusCode));
    response->http_body = str_cat(response->http_body, " - ", 3);
    response->http_body = str_cat(response->http_body, message, strlen(message));
    header->content_length = response->http_body->len;

    if(char_cmp(statusCode, "401", 3, 3)) {
        header->isAuthenticationRequired = 1;
        free_str(response->http_body);
        response->http_body = cpy_str("Authentication Required", 23);

    }

    return response;
}
