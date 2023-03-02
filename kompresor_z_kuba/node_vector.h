#ifndef NODE_VEC
#define NODE_VEC

#include "node.h"

typedef struct Node_vec
{	
	node_t **nodes;
	int n;
	int capacity;
} node_vec_t;

node_vec_t * init_node_vec(int);
int double_node_capacity(node_vec_t*);
int add_node(node_vec_t *, node_t *);
void free_node_vec(node_vec_t*);

#endif
