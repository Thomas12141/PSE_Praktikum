//
// Created by Matteo Illing on 20.04.23.
//

#include <linux/limits.h>
#include "validation.h"

/**
 * Überprüft, ob ein Dateipfad im Dateiverzeichnis ist.
 * Das existierende Dateiverzeichnis wird mit cpy_str() in einen String* überführt und dann mit memcmp verglichen.
 *
 * @param filepath Der zu überprüfende Dateipfad.
 * @return 1, wenn der Dateipfad im Dateiverzeichnis ist, 0 wenn nicht.
 */
int isFileInsideDocroot(char* filepath) {
    char pathBuffer [PATH_MAX+1];
    char* ptr = realpath(DOCROOT, pathBuffer);
    string* docrootPathString = cpy_str(ptr, strlen(ptr));

    int res = memcmp(filepath, docrootPathString->str, docrootPathString->len);
    return res == 0;
}

/**
 * Überprüft, ob die Datei gelesen werden kann, indem versucht wird mit access (Typ R_OK) auf die Datei zuzugreifen.
 *
 * @param filepath Der Dateipfad der zu überprüfenden Datei.
 * @return 1, wenn auf die Datei zugegriffen werden kann, 0 wenn nicht.
 */
int isFileAccessible(char* filepath) {
    if(!access(filepath, R_OK)) {
        return 1;
    }

    return 0;
}

/**
 * Überprüft, ob eine Datei existiert, indem versucht wird mit access (Typ F_OK) auf die Datei zuzugreifen.
 *
 * @param filepath Der Dateipfad der zu überprüfenden Datei.
 * @return 1, wenn die Datei existiert, 0 wenn nicht.
 */
int isFileExistent(char* filepath) {
    if(!access(filepath, F_OK)) {
        return 1;
    }

    return 0;
}

/**
 * Überprüft, ob eine HTTP Methode in Form eines String* die GET Methode ist, indem die übergebene Methode mit "GET" verglichen wird.
 *
 * @param method Die zu verifizierende HTTP-Methode.
 * @return 1, wenn die Methode "GET" ist, 0 wenn nicht.
 */
int isMethodValid(string* method) {
    string* tmpStr = cpy_str("GET", 3);

    if(str_cmp(method, tmpStr)) {
        return 1;
    }

    return 0;
}