#ifndef DECODE_H
#define DECODE_H


#include "../node/node.h"
#include <stdio.h>

char *read_original_extension(FILE*);
node_t *read_dictionary(int*, char**, FILE*);
int read_padding(int*, char**, FILE *file);
int decode_data(int*, char**, FILE*, int*, unsigned char*, FILE*, node_t*, int);
int decode(FILE*, FILE*);


#endif
