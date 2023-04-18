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

#define PORT 31337
#define BUFFER_SIZE 1024*1024
#define DOCROOT "../htdocs/"

/**
 * Überprüft, ob eine Datei existiert.
 * @param filename Dateiname im Docroot.
 * @return Wahrheitswert (1 = datei existiert, 0 = datei existiert nicht).
*/
int isFileExistent(string* filename) {
    //Zusammenfügen von DOCROOT (Verzeichnis) und string (html-Datei) für fopen.
    string* docroot = cpy_str(DOCROOT, strlen(DOCROOT));
    string* file_path = str_cat(docroot, filename->str, filename->len);

    //Prüfen, ob die Datei existiert.
    FILE *file = fopen(get_char_str(file_path), "r");;
    if (file != NULL) {
        fclose(file);
        return 1; // Datei existiert.
    }
    return 0; // Datei existiert nicht.
}

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
         * Der accept()-Aufruf blockiert, bis eine neue Verbindung rein kommt.
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
 * Die Funktion akzeptiert den eingehenden Request und gibt eine entsprechende Response zurück.
 * @param request Der eingehende Request.
 * @return int (1 = ist valid, 0 = ist nicht valid, z.B. außerhalb des docroots).
 */
int isResourceValid(string* resource_path) {
    for (int i = 0; i<= resource_path->len; i++) {   //  Hier läuft die Schleife unsere Request durch
          if(resource_path->str[i] == '.' &&  resource_path->str[i+1] == '.' && resource_path->str[i+2] == '/') {
              return 0; // Überprüft auf die Anwesenheit und Positionen der genannte Zeichen
          }
    }

    return 1;
}
string* process(string *request) {
    /*
     * Diese Funktion müssen Sie anpassen, so dass der request von Ihrem Code verarbeitet wird,
     * die response generiert und zurück gibt.
     *
     * Für den Echo-Server wird der request einfach als response zurückgegeben, das Echo eben.
     */

    string *response = request;
    return response;
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
