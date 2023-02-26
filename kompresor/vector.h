#ifndef VECTOR_H
#define VECTOR_H

#include "node.h"

typedef struct
{
	node_t **nodes;
	int n;
	int memory;
} vector_t;

vector_t *init_vector();

int increase_memory(vector_t *);

int add_element(vector_t*, node_t*);

void free_vector(vector_t*);

#endif
