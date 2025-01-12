//
// Created by Matteo Illing on 20.04.23.
//

#include "strlib.h"

/**
 * Entfernt hexadezimale Zeichen von einem String*, beispielsweise "%20" = " ".
 * Das Nullterminierungszeichen wird aus dem Pfad entfernt.
 *
 * @author Thomas Fidorin
 * @param str String, in dem die Zeichen ersetzt werden sollen.
 * @return string* Der String mit dekodierten Sonderzeichen.
 */
string* decodeString(string* str) {
    int isDecoded = 0;
    while(!isDecoded) {
        isDecoded = 1;
        int i = 0;
        while(i < str->len) {
            if(str->str[i] == '%' && i + 2 < str->len) {
                int temp = 0;
                if(str->str[i + 1] > 49 && str->str[i + 1] < 58) {
                    temp += str->str[i + 1] - 48;
                } else if(str->str[i + 1] > 97 && str->str[i + 1] < 103) {
                    temp += str->str[i + 1] - 87;
                } else if(str->str[i + 1] > 64 && str->str[i + 1] < 71) {
                    temp += str->str[i + 1] - 55;
                }
                temp *= 16;
                if(str->str[i + 2] > 47 && str->str[i + 2] < 58) {
                    temp += str->str[i + 2] - 48;
                } else if(str->str[i + 2] > 64 && str->str[i + 2] < 71) {
                    temp += str->str[i + 2] - 55;
                } else if(str->str[i + 1] > 97 && str->str[i + 1] < 103) {
                    temp += str->str[i + 1] - 87;
                }
                if(temp < 32 || temp > 255) {
                    ++i;
                    continue;
                }
                for (int k = 0; k < 2; ++k) {
                    for (int j = i; j < str->len - 1; ++j) {
                        str->str[j] = str->str[j + 1];
                    }
                }
                isDecoded = 0;
                str->str[i] = temp;
                str->len -= 2;
                str->str[str->len] = '\0';
                str->str = realloc(str->str, str->len);
            } else if(str->str[i] == '+') {
                str->str[i] = ' ';
            }
            ++i;
        }
    }
    return str;
}

/**
 * Hängt einen String* src mit der Länge len an einen bestehenden String* dest an.
 *
 * @author Jeremy Beltran
 * @param dest An diesen String* wird angehängt.
 * @param src Dieser String wird an dest angehängt.
 * @param len Die Länge von src.
 * @return Die konkatenierten Strings dest und src als String*.
 */
string* str_cat(string* dest, const char* src, size_t len) {
    size_t total_len = dest->len + len;

    dest->str = realloc(dest->str, total_len + 1);
    if(dest->str == NULL) {
        exit(3);
    }

    memcpy(dest->str + dest->len, src, len);

    dest->str[total_len] = '\0';
    dest->len = total_len;

    return dest;
}

/**
 * Erstellt einen neuen leeren String und gibt das Ergebnis zurück.
 * Im Fehlerfall (z. B. kein Speicher verfügbar), wird das Programm
 * beendet. Der neue string* muss am Ende gefreit werden.
 *
 * Dies ist eine "interne" Funktion, die nur innerhalb der strlib
 * aufgerufen werden sollte.
 *
 * @return string* Der neue leere String.
 */
string* _new_string() {
    string* str = calloc(sizeof(string), 1);
    if(str == NULL) {
        exit(3);
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
 * @param str Der auszugebende string*.
 */
void print_string(string* str) {
    for(int i = 0; i < str->len; ++i) {
        putchar(str->str[i]);
    }
}

/**
 * Wandelt einen char-String src mit einer Länge len um
 * in einen String vom Typ string*. Im Fehlerfall wird das
 * Programm beendet. Der string* muss am Ende gefreit werden.
 *
 * @param src Der Quell-string.
 * @param len Die Länge des Quell-Strings.
 * @return string* Ein Zeiger auf den neu erzeugten string.
 */
string* cpy_str(const char* src, size_t len) {
    assert(src != NULL);

    string* dest = calloc(sizeof(string), 1);
    if(dest == NULL) {
        exit(3);
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
 * Gibt den String* str frei.
 *
 * @param str Der freizugebende String vom Typ String*.
 */
void free_str(string* str) {
    assert(str != NULL);
    assert(str->str != NULL);
    free(str->str);
    free(str);
}

/**
 * Gibt die Länge des Strings* str zurück.
 *
 * @param str Der String*.
 * @return size_t Die Länge des Strings.
 */
size_t get_length(string* str) {
    assert(str != NULL);
    return str->len;
}

/**
 * Gibt einen Zeiger auf das char-Array des strings* str zurück.
 * Achtung, dieses char-Array ist nicht "null-terminiert"!
 *
 * @param str Der String*.
 * @return char* Der Zeiger auf das char-Arrays des String.
 */
char* get_char_str(string* str) {
    assert(str != NULL);
    return str->str;
}

/**
 * Vergleicht zwei Objekte des Typs string*, indem zuerst die Länge und dann der Inhalt verglichen wird.
 *
 * @param str1 Ein String*.
 * @param str2 Ein String*.
 * @return 1, wenn die Strings gleich sind und 0 wenn sie ungleich sind.
 */
int str_cmp(string* str1, string* str2) {
    if (str1->len != str2->len) {
        return 0;
    }

    if (memcmp(str1->str, str2->str, str1->len) == 0) {
        return 1;
    }

    return 0;
}

/**
 * Wandelt alle Großbuchstaben eines string* in Kleinbuchstaben um.
 *
 * @author Jeremy Beltran
 * @param src Der String*.
 * @return Der modifizierte String* in lowercase.
 */
string* str_lower(string* src) {
    for (int i = 0; i < src->len; ++i) {
        if (src->str[i] >= 'A' && src->str[i] <= 'Z') {
            src->str[i] = src->str[i] + 32;
        }
    }

    return src;
}

/**
 * Vergleicht zwei Objekte des Typs char*, indem die Länge und der Inhalt verglichen wird.
 *
 * @author Jeremy Beltran
 * @param str1 Ein char*.
 * @param str2 Ein char*.
 * @return 1, wenn die Strings gleich sind und 0 wenn sie ungleich sind.
 */
int char_cmp(char* str1, char* str2, size_t len1, size_t len2) {
    if (len1 != len2) {
        return 0;
    }

    if (memcmp(str1, str2, len1) == 0) {
        return 1;
    }

    return 0;
}