#ifndef ENCODE_H
#define ENCODE_H


#include "../vector/node_vector.h"
#include <stdio.h>

char *find_code(int, node_vec_t*);
void encode(FILE*, FILE*, char*, node_vec_t*);


#endif
