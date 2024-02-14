#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include "../heap/heap_min.h"
#include "../node/node.h"

int build_heap_queue(heap_min_t*, char*);
node_t *make_tree(heap_min_t*);
void free_tree(node_t*);


#endif
