//
// Created by Matteo Illing on 27.03.23.
//

#include "httplib.h"
#include <assert.h>
#include <stdbool.h>

/**
 * Hängt einen String src mit der Länge len an einen bestehenden String dest an.
 *
 * @param dest An diesen String wird angehängt.
 * @param src Dieser String wird an dest angehängt.
 * @param len Die Länge von src.
 * @return string* Die konkatenierten Strings dest und src.
 */
string* str_cat(string* dest, const char* src, size_t len) {

    //Berechnet die neue länge des Strings
    size_t total_len = dest->len + len;

    //Der reservierte  Speicher von des->str wird auf die benötigte länge angepasst
    dest->str = realloc(dest->str, total_len);

    //kopiert 'len' Bytes aus src auf die letzte Position von dest->str.
    memcpy(dest->str + dest->len,src, len);

    //Passt die länge von dest an
    dest->len = total_len;

    return dest;
}

/**
 * Erstellt einen neuen leeren String und gibt das Ergebnis zurück.
 * Im Fehlerfall (z. B. kein Speicher verfügbar), wird das Programm
 * beendet.
 *
 * Dies ist eine "interne" Funktion, die nur innerhalb der HTTPLib
 * aufgerufen werden sollte.
 *
 * @return string* Der neue leere String.
 */
string* _new_string() {
    string* str = calloc(sizeof(string), 1);
    if(str == NULL) {
        exit(2);
    }

    str->str = calloc(1, 1);
    if(str->str == NULL) {
        exit(3);
    }

    str->str[0] = '\0';

    str->len = 0;

    return str;
}

/**
 * Gibt einen String auf stdout aus.
 *
 * @param str Der auszugebende String.
 */
void print_string(string* str) {
    for(int i=0; i < str->len; i++) {
        putchar(str->str[i]);
    }
}

/**
 * Wandelt einen char-String src mit einer Länge len um
 * in einen String vom Typ *string. Im Fehlerfall wird das
 * Programm beendet.
 *
 * @param src Der Quell-String.
 * @param len Die Länge des Quell-Strings.
 * @return string* Ein Zeiger auf den neu erzeugten string.
 */
string* cpy_str(const char* src, size_t len) {

    assert(src != NULL);

    string* dest = calloc(sizeof(string), 1);
    if(dest == NULL) {
        exit(2);
    }

    dest->str = calloc(1, len);
    if(dest->str == NULL) {
        exit(3);
    }
    memcpy(dest->str, src, len);
    dest->len = len;

    return dest;
}

/**
 * Gibt den String str frei.
 *
 * @param str Der freizugebende String.
 */
void free_str(string* str) {
    assert(str != NULL);
    assert(str->str != NULL);
    free(str->str);
    free(str);
}

/**
 * Gibt die Länge des Strings str zurück.
 *
 * @param str Der String.
 * @return size_t Die Länge des Strings.
 */
size_t get_length(string* str) {
    assert(str != NULL);
    return str->len;
}

/**
 * Gibt einen Zeiger auf das char-Array des Strings str zurück.
 * Achtung, dieses char-Array ist nicht "null-terminiert"!
 *
 * @param str Der String.
 * @return char* Der Zeiger auf das char-Arrrays des String.
 */
char* get_char_str(string* str) {
    assert(str != NULL);
    return str->str;
}


http_request* getRequestStruct(string* request_string){
    http_request* request = calloc(sizeof(http_request), 1);
    if(request == NULL) {
        exit(2);
    }

        int positionen[5];
        int j = 0;

        for (int i = 0; i < request_string->len; i++) {
            if (j == 5) {
                break;
            }

            if ((request_string->str[i+1] == ' ' &&  i+1 !=  i < request_string->len)|| request_string->str[i++] == '\r') {
                positionen[j] = i;
                j++;
                continue;
            }

            if (request_string->str[i-1] == ' ') {
                positionen[j] = i;
                j++;
            }
        }

        request->method->str = realloc(request->method->str, positionen[0] + 1);
        request->resource_path->str = realloc(request->resource_path->str, positionen[2] - positionen[1] + 1);
        request->protocol->str = realloc(request->protocol->str, positionen[4] - positionen[3] + 1);

        memcpy(request->method->str, request_string->str, positionen[0] + 1);
        request->method->len = positionen[0] + 1;
        memcpy(request->resource_path->str, request_string->str + positionen[1], positionen[2] - positionen[1] + 1);
        request->resource_path->len = positionen[2] - positionen[1] + 1;
        memcpy(request->protocol->str, request_string->str + positionen[3], positionen[4] - positionen[3] + 1);
        request->resource_path->len = positionen[4] - positionen[3] + 1;

    return request;
}
