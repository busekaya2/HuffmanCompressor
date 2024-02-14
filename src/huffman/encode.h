#ifndef ENCODE_H
#define ENCODE_H


#include "../map/hashmap.h"
#include "../node/node.h"
#include <stdio.h>

int encode(char*, char*, char*, node_t*, hashmap_t*);
void encode_dictionary(int*, unsigned char*, FILE*, node_t*);
void encode_data(int*, unsigned char*, FILE*, FILE*, hashmap_t*);
void encode_padding(FILE*, int);


#endif
