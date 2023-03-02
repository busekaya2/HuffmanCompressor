#ifndef HUFFMAN
#define HUFFMAN

#include "node.h"
#include "node_vector.h"

int node_cmp(const void *, const void *);
node_t * make_tree(node_vec_t *);
void read_codes(node_t*, node_vec_t*, char*);
void free_tree(node_t*);

#endif
