#include <stdint.h>
#include <stdlib.h>

void build_decoding_table1();
char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);
void base64_cleanup();