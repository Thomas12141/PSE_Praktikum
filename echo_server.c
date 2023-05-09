#include <errno.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "httplib.h"
#include "validation.h"
#include "config.h"
#include "http_status.h"

string* process(string *request);

static bool run = true;

/**
 * Gibt eine Fehlermeldung *msg* aus und beendet das Programm.
 * @param msg Die Fehlermeldung.
 */
static void error(char *msg) {
    fprintf(stderr, "%s", msg);
    if (errno) {
        fprintf(stderr, ", errno: %s", strerror(errno));
    }
    fprintf(stderr, "\n");
    exit(1);
}

/**
 * Diese Funktion wird aufgerufen, wenn das Programm das *SIGINT*-Signal empfängt. Es beendet den Server.
 * @param signum Die Signalnummer.
 */
static void handle_signal(int signum) {
    if (signum != SIGINT) {
        error("ERROR unexpected signal");
    }

    /*
     * Beende den Server nach dem Abarbeiten des letzten Clients.
     */
    run = false;
}

/**
 * Registriert das SIGINT-Signal (Strg+C) um den Server beenden zu können.
 */
static void register_signal() {
    struct sigaction action;

    /*
     * Konfigurieren des Signal-Handlers.
     */
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;

    /*
     * Registrierung des Signal-Handlers.
     */
    if (sigaction(SIGINT, &action, NULL) < 0) {
        error("ERROR registering signal handler");
    }
}

/**
 * Erstellt und konfiguriert den Netzwerk-Socket, über den die Verbindungen
 * angenommen werden.
 */
static int setup_socket() {
    int opt = 1;
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    /*
     * Setzt Konfigurationsvariablen für den Socket, z.B. die Portnummer.
     */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    /*
     * Erstelle den Socket.
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    /*
     * Verwende den Socket, selbst wenn er aus einer vorigen Ausführung
     * im TIME_WAIT Status ist.
     */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt,
                   sizeof(int)) < 0)
        error("ERROR on setsockopt");

    /*
     * Melde, dass der Socket eingehende Verbindungen akzeptieren soll.
     */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    /*
     * Horche auf dem Socket nach eingehenden Verbindungen. Es werden maximal
     * fünf gleichzeitige Verbindungen erlaubt.
     */
    if (listen(sockfd, 5) < 0) {
        error("listen");
    }
    return sockfd;
}

/**
 * Liest Daten von stdin ein, überführt sie in einen string* und übergibt sie an process().
 * Wenn ein Fehler auftritt wird error() aufgerufen.
 */
static void main_loop_stdin() {
    void *const buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        error("ERROR at malloc.");
    }

    /*
     * Lies die ankommenden Daten von dem Socket in das Array buffer.
     */
    memset(buffer, 0, BUFFER_SIZE);
    size_t length = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
    if (length < 0) {
        if (errno != EINTR) {
            error("ERROR reading from socket");
        }
    }
    string *request = cpy_str(buffer, length);
    string *response = process(request);

    size_t response_len = get_length(response);
    char *response_char = get_char_str(response);
    /*
     * Schreibe die ausgehenden Daten auf stdout.
     */
    if (write(STDOUT_FILENO, response_char, response_len) < 0) {
        error("ERROR writing to STDOUT");
    }

    free_str(response);
    free(buffer);
}

/**
 * Die Hauptschleife, in der eingehende Verbindungen angenommen werden.
 */
static void main_loop() {
    const int sockfd = setup_socket();
    int newsockfd;
    ssize_t length;

    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    void *const buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        error("ERROR at malloc.");
    }

    /*
     * Die Hauptschleife des Programms.
     */
    while (run) {

        /*
         * Der accept()-Aufruf blockiert, bis eine neue Verbindung reinkommt.
         */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            /*
             * Wenn der Server mit dem SIGINT-Signal beendet wird, schlägt accept
             * mit EINTR (interrupted) fehl.
             */
            if (errno == EINTR) {
                break;
            }
            error("ERROR on accept");
        }

        /*
         * Lies die ankommenden Daten von dem Socket in das Array buffer.
         */
        memset(buffer, 0, BUFFER_SIZE);
        length = read(newsockfd, buffer, BUFFER_SIZE - 1);
        if (length < 0) {
            if (errno == EINTR) {
                break;
            }
            error("ERROR reading from socket");
        }
        string *request = cpy_str(buffer, length);
        string *response = process(request);

        /*
         * Schreibe die ausgehenden Daten auf den Socket.
         */
        size_t response_len = get_length(response);
        char *response_char = get_char_str(response);
        length = write(newsockfd, response_char, response_len);
        if (length < 0) {
            error("ERROR writing to socket");
        }
        free_str(response);
        /*
         * Schließe die Verbindung.
         */
        if (close(newsockfd) < 0) {
            error("ERROR on close");
        }
    }

    free(buffer);
    if (close(sockfd) < 0) {
        error("ERROR on close");
    }
}

/**
 * Verarbeitet den übergebenen request-String* struct und gibt eine entsprechende http_response* als string* struct zurück.
 * Wenn die http_request ungültig ist, besteht die http_response aus, den Fehlern entsprechenden, Statusmeldungen.
 *
 * @author Matteo Illing
 * @param request Der http_request struct.
 * @return Der http_response struct.
 */
string* process(string* request) {
    http_request* requestStruct = getRequestStruct(request);
    free_str(request);

    if(requestStruct == NULL) {
        return getResponseString(getShortResponse("400", HTTP_400_MESSAGE));
    }

    if(!isProtocolValid(requestStruct->protocol)) {
        freeRequestStruct(requestStruct);
        return getResponseString(getShortResponse("505", HTTP_505_MESSAGE));
    }

    if(isAuthenticationRequired(requestStruct->hostname)) {
        freeRequestStruct(requestStruct);
        return getResponseString(getShortResponse("401", HTTP_401_MESSAGE));
    }

    if(char_cmp(requestStruct->resource_path->str, "/debug", requestStruct->resource_path->len, 6)) {
        http_response* res = getShortResponse("200", HTTP_200_MESSAGE);
        free_str(res->http_body);
        res->http_body = cpy_str(requestStruct->method->str, requestStruct->method->len);
        res->http_body = str_cat(res->http_body, " ", 1);
        res->http_body = str_cat(res->http_body,requestStruct->resource_path->str,requestStruct->resource_path->len);
        res->http_body = str_cat(res->http_body, " ", 1);
        res->http_body = str_cat(res->http_body,requestStruct->protocol->str,requestStruct->protocol->len);
        res->header->content_length = res->http_body->len;

        freeRequestStruct(requestStruct);

        return getResponseString(res);
    }

    sanitizeRequestedResource(requestStruct);
    char* secureFilepath = getFilePath(requestStruct);

    if(!secureFilepath || !isFileInsideDocroot(secureFilepath, requestStruct->hostname)) {
        freeRequestStruct(requestStruct);
        free(secureFilepath);
        return getResponseString(getShortResponse("403", HTTP_403_MESSAGE));
    }

    if(!isFileExistent(secureFilepath)) {
        freeRequestStruct(requestStruct);
        free(secureFilepath);
        return getResponseString(getShortResponse("404", HTTP_404_MESSAGE));
    }

    if (!isFileAccessible(secureFilepath)) {
        freeRequestStruct(requestStruct);
        free(secureFilepath);
        return getResponseString(getShortResponse("403", HTTP_403_MESSAGE));
    }

    if (!isMethodValid(requestStruct->method)) {
        free(secureFilepath);
        freeRequestStruct(requestStruct);
        return getResponseString(getShortResponse("501", HTTP_501_MESSAGE));
    }

    string* file = readFile(secureFilepath);
    if(file == NULL) {
        freeRequestStruct(requestStruct);
        free(secureFilepath);
        return getResponseString(getShortResponse("403", HTTP_403_MESSAGE));
    }

    http_response* res = getShortResponse("200", HTTP_200_MESSAGE);

    string* filetype = getFiletype(secureFilepath, strlen(secureFilepath));
    string* contenttype = getContentType(filetype);
    free(secureFilepath);
    freeRequestStruct(requestStruct);

    free_str(res->header->content_type);
    res->header->content_type = contenttype;
    res->header->content_length = file->len;
    free_str(res->http_body);
    res->http_body = file;

    return getResponseString(res);
}

int main(int argc, char *argv[]) {
    register_signal();

    if(argc == 2 && strcmp("stdin", argv[1]) == 0) {
        main_loop_stdin();
    } else {
        main_loop();
    }

    return 0;
}
