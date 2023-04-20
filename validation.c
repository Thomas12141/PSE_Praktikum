//
// Created by Matteo Illing on 20.04.23.
//

#include "validation.h"

/**
 * Die Funktion akzeptiert den eingehenden Request und gibt eine entsprechende Response zurück.
 * @param request Der eingehende Request.
 * @return int (1 = ist valid, 0 = ist nicht valid, z.B. außerhalb des docroots).
 */
int isResourceValid(string* resource_path) {
    char pathBuffer [PATH_MAX+1];
    char* ptr = realpath(DOCROOT, pathBuffer);

    string* docroot = cpy_str(DOCROOT, strlen(DOCROOT));
    string* file_path = str_cat(docroot, resource_path->str, resource_path->len);
    string* docrootPathString = cpy_str(ptr, strlen(ptr));

    char* filepathPointer = calloc(file_path->len + 1, 1);
    memcpy(filepathPointer, file_path->str, file_path->len);

    char* requestedFilePath = realpath(filepathPointer, pathBuffer);
    string* filePathString = cpy_str(requestedFilePath, strlen(requestedFilePath));
    free(filepathPointer);

    int res = memcmp(filePathString->str, docrootPathString->str, docrootPathString->len);

    return res == 0;
}


/**
 * Überprüft, ob eine Datei existiert.
 * @param filename Dateiname im Docroot.
 * @return Wahrheitswert (1 = datei existiert, 0 = datei existiert nicht).
*/
int isFileExistent(string* filename) {
    //Zusammenfügen von DOCROOT (Verzeichnis) und string (html-Datei) für fopen.
    string* docroot = cpy_str(DOCROOT, strlen(DOCROOT));
    string* file_path = str_cat(docroot, filename->str, filename->len);

    char* filepathPointer = calloc(file_path->len + 1, 1);
    memcpy(filepathPointer, file_path->str, file_path->len);

    //Prüfen, ob die Datei existiert.
    FILE *file = fopen(filepathPointer, "r");;
    if (file != NULL) {
        fclose(file);
        return 1; // Datei existiert.
    }
    return 0; // Datei existiert nicht.
}