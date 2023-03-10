#ifndef DECODE
#define DECODE

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

char* byte_to_binary(unsigned char);
void decode(FILE*, FILE*, node_t*);

#endif
