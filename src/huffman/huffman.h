#ifndef HUFFMAN_H
#define HUFFMAN_H


#include "../vector/node_vector.h"
#include "../heap/heap_min.h"
#include "../node/node.h"
#include <stdio.h>

#define BYTE_SIZE 256

int build_heap_content(FILE*, heap_min_t*);
node_t * make_tree(heap_min_t*);
void read_codes(node_t*, node_vec_t*, char*);
void free_tree(node_t*);


#endif
