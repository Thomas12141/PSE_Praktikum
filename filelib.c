//
// Created by Matteo Illing on 08.05.23.
//

#include "filelib.h"

/**
 * Ließt den Inhalt einer Datei filepath ein und gibt diesen als string* zurück.
 * Der Inhalt wird in char* buffer eingelesen und dann mit cpy_str in ein String* umgewandelt.
 *
 * @param filepath Der Dateipfad.
 * @return string* Dateiinhalt.
 */
string* readFile(char* filepath) {
    char* buffer;
    unsigned long fileSize = 0;

    FILE* file = fopen(filepath, "rb");

    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file);
    rewind(file);

    buffer = calloc(fileSize, 1);
    if(buffer == NULL) {
        return NULL;
    }

    fread(buffer, 1, fileSize, file);
    fclose(file);

    string* fileContent = cpy_str(buffer, fileSize);
    free(buffer);

    return fileContent;
}

/**
 * Konstruiert den Dateipfad für das übergebene Objekt des Typs http_request*.
 *
 * @param request Das http_request*-Objekt, für das der Dateipfad konstruiert werden soll.
 * @return Der Dateipfad als char*.
 */
char* getFilePath(http_request* request) {
    char* pathBufferLong = calloc(PATH_MAX, 1);
    string* file_path = getDocrootpath(request->hostname);
    file_path = str_cat(file_path, request->resource_path->str, request->resource_path->len);

    char* filepathPointer = calloc(file_path->len + 1, 1);
    memcpy(filepathPointer, file_path->str, file_path->len);
    free_str(file_path);

    realpath(filepathPointer, pathBufferLong);
    free(filepathPointer);

    unsigned long pathLength = strlen(pathBufferLong);
    char* shortenedPath = calloc(strlen(pathBufferLong) + 1, 1);
    memcpy(shortenedPath, pathBufferLong, pathLength);

    free(pathBufferLong);

    return shortenedPath;
}

/**
 * Ermittelt den fileType (Dateiendung) einer Datei.
 *
 * @param resource_path Der Dateipfad der betroffenen Datei als char*.
 * @param len Die Länge des Dateipfads.
 * @return Den fileType ohne Punkt als string*.
 */
string* getFiletype (char* resource_path, int len) {

    int dot_position = 0;
    string* content_type;

    for (int i = len; i >= 0; i--){
        if(resource_path[i] == '/') {
            break;
        }
        if (resource_path[i] == '.') {
            dot_position = i;
        }
    }

    //check if there is no dot in the file path
    if(dot_position == 0) {
        content_type = cpy_str("txt", 3);
    } else {
        content_type = str_lower(cpy_str(resource_path+dot_position+1, len - dot_position - 1));
    }

    return content_type;
}

string* getDocrootpath(string* hostname) {
    char pathBuffer [PATH_MAX+1];
    char* ptr = realpath(DOCROOT, pathBuffer);
    string* docrootPathString = str_cat(cpy_str(ptr, strlen(ptr)), "/", 1);

    string* intern_str = cpy_str("intern", 6);
    string* extern_str = cpy_str("extern", 6);
    if(str_cmp(hostname, intern_str) || str_cmp(hostname, extern_str)){
        docrootPathString = str_cat(docrootPathString, hostname->str, hostname->len);
    } else
        docrootPathString = str_cat(docrootPathString, "default", 7);

    free_str(intern_str);
    free_str(extern_str);
    return docrootPathString;
}
