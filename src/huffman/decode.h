#ifndef DECODE_H
#define DECODE_H


#include "../node/node.h"
#include <stdio.h>

char *read_original_extension(FILE*);
node_t *read_dictionary(int*, unsigned char*, FILE*);
int read_padding(int*, unsigned char*, FILE *file);
int decode_data(int*, unsigned char*, FILE*, FILE*, node_t*, int);
int decode(FILE*, FILE*);


#endif
