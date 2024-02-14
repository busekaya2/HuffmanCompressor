#ifndef READ_CODES_H
#define READ_CODES_H


#include "../map/hashmap.h"
#include "../node/node.h"

int read_codes(node_t*, hashmap_t*);
void read_codes_recursive(node_t*, char*, hashmap_t*);


#endif