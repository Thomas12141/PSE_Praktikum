//
// Created by Matteo Illing on 20.04.23.
//

#include "validation.h"

int isFileInsideDocroot(char* filepath) {
    char pathBuffer [PATH_MAX+1];
    char* ptr = realpath(DOCROOT, pathBuffer);
    string* docrootPathString = cpy_str(ptr, strlen(ptr));

    int res = memcmp(filepath, docrootPathString->str, docrootPathString->len);
    return res == 0;
}

int isFileAccessible(char* filepath) {
    if(!access(filepath, R_OK)) {
        return 1;
    }

    return 0;
}

int isFileExistent(char* filepath) {
    if(!access(filepath, F_OK)) {
        return 1;
    }

    return 0;
}

int isMethodValid(string* method) {
    string* tmpStr = cpy_str("GET", 3);

    if(str_cmp(method, tmpStr)) {
        return 1;
    }

    return 0;
}