#include "filelib.h"

/**
 * Ließt den Inhalt einer Datei filepath ein und gibt diesen als string* zurück.
 * Der Inhalt wird in char* buffer eingelesen und dann mit cpy_str in ein String* umgewandelt.
 * Bei Dateien großer als 512MB oder bei einem Lesenfehler oder bei einem Ordner wird NULL zurückgegeben.
 * Diese String muss am Ende gefreit. Wenn es nicht NULL zurückgibt.
 *
 * @author Matteo Illing, Thomas Fidorin
 * @param filepath Der Dateipfad.
 * @return string* Dateiinhalt oder NULL, wenn die Datei zu groß oder nicht vorhanden.
 */
string* readFile(char* filepath) {
    char* buffer;
    unsigned long fileSize = 0;

    FILE* file = fopen(filepath, "rb");
    if(file == NULL) {
        return NULL;
    }
    struct stat sb;
    if( stat( filepath, &sb) != -1&&S_ISREG( sb.st_mode ) == 0){
        return NULL;
    }
    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file);
    rewind(file);


    if(fileSize > 0x20000000000) { // 512 MB
        fclose(file);
        return NULL;
    }

    buffer = calloc(fileSize, 1);
    if(buffer == NULL) {
        fclose(file);
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
 * Die zurückgegebene pointer muss am Ende gefreit werden.
 *
 * @author Matteo Illing
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
 * Ermittelt den fileType (Dateiendung) einer Datei. Wenn der nicht vorhanden ist, wird txt zurückgegeben.
 * Der Pfad muss an Ende gefreit werden.
 *
 * @author Simon Lammers
 * @param resource_path Der Dateipfad der betroffenen Datei als char*.
 * @param len Die Länge des Dateipfads.
 * @return Den fileType ohne Punkt als string*. Wenn der nicht vorhanden ist, wird txt zurückgegeben.
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

/**
 * Gibt den Docroot-Pfad abhängig vom Hostnamen als String* zurück.
 * Wenn der Hostname nicht "intern" oder "extern" entspricht, wird der Default-Dateipfad verwendet.
 * Falls der Docroot nicht existiert, wird Null zurückgegeben.
 * Wenn die Rückgabe nicht NULL ist, muss der String später gefreit werden.
 *
 * @author Jeremy Beltran,Thomas Fidorin
 * @param hostname Der Hostname als string*.
 * @return Der Docroot-Pfad als string*. Null, wenn der Docroot nicht existiert.
 */
string* getDocrootpath(string* hostname) {
    char pathBuffer [PATH_MAX+1];
    char* ptr = realpath(DOCROOT, pathBuffer);
    if(ptr==NULL){
        return NULL;
    }
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

/**
 * Prüft ob die Datei größer als 512MB. Im Fall, dass die Datei nicht vorhanden oder kleiner als 512MB ist, wird false zurückgegeben.
 *
 * @author Thomas Fidorin
 * @param path Dateipfad
 * @return 1 für richtig, 0 für falsch
 */
int ifFileTooBig(char * path){

    FILE* file = fopen(path, "rb");
    if(file == NULL) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    unsigned long fileSize = ftell(file);
    if(fileSize > 0x20000000000) { // 512 MB
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}