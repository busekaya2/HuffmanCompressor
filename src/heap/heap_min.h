#ifndef HEAP_MIN_H
#define HEAP_MIN_H


#include "../node/node.h"

typedef struct {
    node_t **elements;
    int capacity;
    int n;
} heap_min_t;

heap_min_t *init_heap_min(int);
int heap_min_insert(heap_min_t*, node_t*);
node_t *heap_min_remove(heap_min_t*);
void heap_min_heapify(heap_min_t*, int);
void heap_min_free(heap_min_t*);
void heap_min_swap(heap_min_t*, int, int);


#endif