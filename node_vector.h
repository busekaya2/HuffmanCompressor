#ifndef NODE_VEC_H
#define NODE_VEC_H


#include "node.h"

/* Struktura dynamicznego wektora przechowującego wskaźniki na węzły.
 * UWAGA - Wektor nie odpowiada za zwalnianie pamięci zawartości, należy to robić ręcznie*/

typedef struct Node_vec{	
	node_t **nodes;
	int n;
	int capacity;
} node_vec_t;

node_vec_t * init_node_vec(int);
int double_node_capacity(node_vec_t*);
int add_node(node_vec_t *, node_t *);
int remove_node(node_vec_t *, int);
void free_node_vec(node_vec_t*);


#endif
