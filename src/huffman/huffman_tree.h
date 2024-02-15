#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include "../heap/heap_min.h"
#include "../node/node.h"

int build_heap_queue(heap_min_t*, char*);
node_t *make_tree(heap_min_t*);
int get_tree_height(node_t*);
void free_tree(node_t*);

typedef struct {
    int code;
    int length;
} huffman_code_t;

void add_bit(huffman_code_t*, unsigned int);
void clear_bit(huffman_code_t*);
int get_bit_at(huffman_code_t*, int);


#endif
