#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "vector.h"


char* byte_to_binary(unsigned char byte);

void decode(FILE *in, FILE *out, vector_t *codes, char*);
