//
// Created by Matteo Illing on 27.03.23.
//

#include "httplib.h"
#include <assert.h>

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

int isMethodValid(http_request* request){

// Erlaubte Methoden
const char methoden[3][4] = {{'G', 'E','T'},{'P', 'O','S','T'},{'H', 'E','A','D'}};

// Pruefft Bedinung der Rueckgabe  0 = False; 1 = True;

/*
 * Die j-Schleife durchlaueft das außere Array von methoden (wechselt das wort)
 * Die i-Schleife durchlaueft den String und das innere Array von methoden
 */
    for (int j = 0; j < 3; j++) {
        for(int i = 0; i < request->method->len; i++){

            //  Prueft ob die character von String und methoden an der Position i gleich sind. Wenn nicht: j++
            if (request->method->str[i] != methoden[j][i] || (j == 0 && i > 2)){
                break;
             }

            // Wenn alle zeichen gleich von String gleich sind,.
            if (i++ == request->method->len){
                return 1;
            }
        }
    }

 return 0;
}

    // Funktion zur Methoden Validierung
int isMethodValid2(http_request* request){

    // Erlaubte HTTP-Methoden
    const char methode_get[3] = {'G', 'E','T'};
    const char methoden[2][4] = {{'P', 'O','S','T'},{'H', 'E','A','D'}};

    /* Prueft ob die Methode die laenge = 3 besitzt.
     * -> Wenn True: durchlaueft die i-Schleife den String und das Array von methode_get.
     * Prueft ob die character von String und methoden an der Position i gleich sind.
     * -> Wenn False: Methode ist nicht erlaubt.
     * -> Wenn True: Methode ist erlaubt.
     */
    if(request->method->len == 3) {
        for (int i = 0; i < 3; i++) {
            if(request->method->str[i] != methode_get[i]){
                return 0;
            }
        }
        return 1;
    }

    /* Prueft ob die Methode die laenge = 4 besitzt.
     * -> Wenn False: Methode ist nicht erlaubt. (Da die Methode die laenge =  3 || 4 haben muss)
     * -> Wenn True: durchlaueft die j-Schleife das außere Array von methoden (Wechselt das Wort)
     * i-Schleife durchlaueft den String und das innere Array von methoden
     * Prueft ob die character von String und methoden an der Position i gleich sind.
     * -> Wenn False: Prüft Naechstes Wort.
     * -> Wenn True: Methode ist erlaubt.
     */
    if(request->method->len == 4)
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 4; i++) {

                if (request->method->str[i] != methoden[j][i]) {
                    break;
                }

                if (i++ == request->method->len) {
                        return 1;
                }
            }
        }

    return 0;
}

