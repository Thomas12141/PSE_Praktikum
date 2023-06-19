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

/**
 * Ein struct, welches dazu dient nicht-nullterminierte String, sammt der Länge, zu speichern.
 * Wobei len die Länge eines Strings abbildet und str den Inhalt.
 */
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
string* decodeString(string* str);
int char_cmp(char* str1, char* str2, size_t len1, size_t len2);
string* str_lower(string* src);

#endif //PSE_WEBSERVER_STRLIB_H
