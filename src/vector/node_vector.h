#ifndef NODE_VEC_H
#define NODE_VEC_H


#include "../node/node.h"

/* Structure of a dynamic vector holding pointers to strings.
 * NOTE - The vector is not responsible for freeing the memory of the contents, it must be done manually */

typedef struct Node_vec{	
	node_t **nodes;
	int n;
	int capacity;
} node_vec_t;

node_vec_t * init_node_vec(int);
int add_node(node_vec_t *, node_t *);
int remove_node(node_vec_t *, int);
void free_node_vec(node_vec_t*);


#endif
