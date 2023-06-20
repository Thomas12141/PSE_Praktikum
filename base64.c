#include "base64.h"



/**
 * Die Base64 Zeichen Tabelle.
 *
 * @author Sebastian Schinzel
 */
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

/**
 * Bildet die Base64 Dekodierungstabelle. Am Ende muss die Tabelle gereit werden.
 *
 * @author Sebastian Schinzel
 */
void build_decoding_table1() {

    int i;
    decoding_table = malloc(256);

    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


/**
 * Die Methode encodiert eine Base64 Zeichenkette.
 * Die Methode prüft nicht, ob die Zeichenkette wirklich Base64.
 * Wenn die Methode eine nicht base64 Zeichenkette bekommt, kann es zu unerwatete Ergebnisse führen.
 * Die Rückgabe muss am Ende gefreit werden.
 *
 * @author Sebastian Schinzel
 * @param data die Zeichekette, die encodiert werden sollte.
 * @param input_length die Länge der Zeichekette, die encodiert werden sollte.
 * @param output_length ein pointe auf die Länge der Zeichekette, die am Ende rauskommen sollte.
 * @return ein Zeiger auf die encodierte Zeichenkette.
 */
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    int i,j;
    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';
    encoded_data[*output_length] = '\0';

    return encoded_data;
}

/**
 * Die Methode decodiert eineZeichenkette zu Base64 Zeichenkette.
 * Die Rückgabe muss am Ende gefreit werden.
 *
 * @author Sebastian Schinzel
 * @param data die Zeichekette, die decodiert werden sollte.
 * @param input_length die Länge der Zeichekette, die decodiert werden sollte.
 * @param output_length ein pointe auf die Länge der Zeichekette, die am Ende rauskommen sollte.
 * @return ein Zeiger auf die decodierte Zeichenkette.
 */
unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    int i, j;
    if (decoding_table == NULL) build_decoding_table1();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length + 1);
    if (decoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    decoded_data[*output_length] = '\0';

    return decoded_data;
}

/**
 * freit die base64 tabelle.
 *
 * @author Sebastian Schinzel
 */
void base64_cleanup() {
    free(decoding_table);
}
