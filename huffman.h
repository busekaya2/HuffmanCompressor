#ifndef HUFFMAN_H
#define HUFFMAN_H


#include "node_vector.h"
#include "heap_min.h"
#include "node.h"

node_t * make_tree(heap_min_t*);
void read_codes(node_t*, node_vec_t*, char*);
void free_tree(node_t*);


#endif
