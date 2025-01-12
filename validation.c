
#include "validation.h"
#include "httplib.h"


/**
 * Überprüft, ob ein Dateipfad im Dateiverzeichnis ist.
 * Das existierende Dateiverzeichnis wird mit cpy_str() in einen String* überführt und dann mit memcmp() verglichen.
 *
 * @author Matteo Illing
 * @param filepath Der zu überprüfende Dateipfad.
 * @param hostname Der Hostname im request-Header
 * @return 1, wenn der Dateipfad im Dateiverzeichnis ist, 0 wenn nicht.
 */
int isFileInsideDocroot(char* filepath, string* hostname) {
    string* docrootPathString = getDocrootpath(hostname);

    if(strlen(filepath) < docrootPathString->len) {
        free_str(docrootPathString);
        return 0;
    }

    int res = memcmp(filepath, docrootPathString->str, docrootPathString->len);
    free_str(docrootPathString);
    return res == 0;
}

/**
 * Überprüft, ob die Datei gelesen werden kann, indem versucht wird mit access (Typ R_OK) auf die Datei zuzugreifen.
 *
 * @author Matteo Illing
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
 * @author Matteo Illing
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
 * @author Jeremy Beltran
 * @param method Die zu verifizierende HTTP-Methode.
 * @return 1, wenn die Methode "GET" ist, 0 wenn nicht.
 */
int isMethodValid(string* method) {
    string* tmpStr = cpy_str("GET", 3);

    if(str_cmp(method, tmpStr)) {
        free_str((tmpStr));
        return 1;
    }

    free_str(tmpStr);
    return 0;
}

/**
 * Überprüft, ob das HTTP Protokoll einer request HTTP-Version 1.1 entspricht.
 *
 * @author Simon Lammers
 * @param protocol Das zu überprüfende Protokoll des Typs string*.
 * @return 1, wenn das Protokoll der Version 1.1 entspricht, 0 wenn nicht.
 */
int isProtocolValid(string* protocol) {
    string* tmpStr = cpy_str("HTTP/1.1", 8);

    if(str_cmp(protocol, tmpStr)) {
        free_str(tmpStr);
        return 1;
    }

    free_str(tmpStr);
    return 0;
}

/**
 * Prüft, ob der Hostname intern ist und eine Authentifizierung benötigt wird.
 *
 * @author Simon Lammers
 * @param hostname ein String*
 * @return 1, wenn eine Authentifizierung benötigt wird, 0 wenn nicht.
 */
int isAuthenticationRequired(http_request *httpRequest) {
    string* tmpStr = cpy_str("intern", 6);

    if(str_cmp(httpRequest->hostname, tmpStr)) {
        free_str(tmpStr);
        return 1;
    }

    free_str(tmpStr);
    return 0;
}

/**
 * Prüft, ob ein String Base64 kodiert ist.
 *
 * @author Thomas Fidorin
 * @param request_string Der request vom Server
 * @return 1 für richtig, 0 für falsch
 */
int istCredentialsNichtBase64Kodiert(http_request * request) {
    for (int i = 0; i < request->credentials->len; ++i) {
        if (!(request->credentials->str[i] == 43 ||
              (request->credentials->str[i] > 47 && request->credentials->str[i] < 58) ||
              request->credentials->str[i] == 92
              || (request->credentials->str[i] > 64 && request->credentials->str[i] < 91) ||
              (request->credentials->str[i] > 96
               && request->credentials->str[i] < 123) ||
              (request->credentials->str[i] == 61))) {
            return 1;
        }
    }
    return 0;
}

/**
 * Prüft, ob Username und Passwort richtig sind.
 *
 * @author Thomas Fidorin, Djordy von Rönn
 * @param request_string Der request vom Server
 * @return 1 für richtig, 0 für falsch
 */
int isPasswordUsernameRight(http_request * request){
    // Prüft, ob die credentials leer sind
    if(request->credentials == NULL || request->credentials->len == 0) {
        return 0;
    }

    string *raw = calloc(sizeof(string), 1);
    if(raw == NULL) {
        exit(3);
    }

    // Prüft auf eine korrekte Base64-Kodierung
    if(istCredentialsNichtBase64Kodiert(request)){
        free(raw);
        return 0;
    }
    build_decoding_table1();
    raw->str = base64_decode(request->credentials->str, request->credentials->len, &raw->len);
    int positionColon = 0;
    if(raw->str == NULL){
        base64_cleanup();
        free(raw);
        return 0;
    }

    while (positionColon < raw->len && raw->str[positionColon] != ':'){positionColon++;}
    if(positionColon == raw->len){
        base64_cleanup();
        free_str(raw);
        return 0;
    }

    string *username = cpy_str(raw->str, positionColon);
    string *password = cpy_str(&raw->str[positionColon + 1], raw->len-positionColon - 1);
    free_str(raw);

    // Hashed die Eingaben
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(password->str,password->len, hash);
    string *hashedPasswort = calloc(sizeof(string), 1);

    hashedPasswort->str = base64_encode(hash, 20, &hashedPasswort->len);
    char pathBuffer [PATH_MAX+1];
    char* temp = realpath(DOCROOT, pathBuffer);
    if (temp == NULL) {
        free_str(username);
        free_str(password);
        free_str(hashedPasswort);
        base64_cleanup();
        return 0;
    }

    string * filePath = cpy_str(temp, strlen(temp));
    str_cat(filePath, "/htpasswd", strlen("/htpasswd"));
    FILE *fptr;
    fptr = fopen(filePath->str, "r");
    string* combined = cpy_str(username->str,username->len);

    free_str(username);
    str_cat(combined, ":{SHA}", strlen(":{SHA}"));
    str_cat(combined, hashedPasswort->str, hashedPasswort->len);
    free_str(password);
    free_str(hashedPasswort);

    char pointer;
    if (fptr == NULL) {
        base64_cleanup();
        return 0;
    }

    do{
        pointer = fgetc(fptr);
        for (int i = 0; i < combined->len; ++i) {
            if (i == combined->len - 1 && combined->str[i] == pointer){
                free_str(combined);
                free_str(filePath);
                fclose(fptr);
                base64_cleanup();
                return 1;
            }else if(combined->str[i] == pointer){
                pointer = fgetc(fptr);
            } else{
                break;
            }
        }
        while (pointer != '\n'&&pointer != EOF){
            pointer = fgetc(fptr);
        };
    } while (pointer != EOF);

    free_str(combined);
    free_str(filePath);
    fclose(fptr);
    base64_cleanup();

    return 0;
}
