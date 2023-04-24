//
// Created by Matteo Illing on 20.04.23.
//

#ifndef PSE_WEBSERVER_STRLIB_H
#define PSE_WEBSERVER_STRLIB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "config.h"

struct string {
    size_t len;
    char* str;
};
typedef struct string string;


string* str_cat(string* dest, const char* src, size_t len);
string* _new_string();
void print_string(string* str);
string* cpy_str(const char* src, size_t len);
void free_str(string* str);
size_t get_length(string* str);
char* get_char_str(string* str);
int str_cmp(string* str1, string* str2);
string* readFile(char* filepath);
string* decodeString(string* str);

#endif //PSE_WEBSERVER_STRLIB_H
