#ifndef ENCODE
#define ENCODE

#include "node_vector.h"

char *find_code(char, node_vec_t*);
void encode(FILE*, FILE*, char*, node_vec_t*);

#endif
