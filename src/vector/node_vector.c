#include "node_vector.h"
#include "../node/node.h"
#include <stdlib.h> 


/* Structure of a dynamic vector holding pointers to strings.
 * NOTE - The vector is not responsible for freeing the memory of the contents, it must be done manually */

node_vec_t * init_node_vec(int capacity) {
	node_vec_t *new = malloc(sizeof(node_vec_t));
	if (new == NULL) {
		// Memory alloc error
		return NULL;
	}
	
	new->nodes = malloc(sizeof(node_vec_t*) * capacity);
	if (new->nodes == NULL) {
		// Memory alloc error
		return NULL;
	}

	new->capacity = capacity;
	new->n = 0;
	
	return new;
}

int double_node_capacity(node_vec_t* vec) {
	vec->capacity *= 2;
	vec->nodes = realloc(vec->nodes, sizeof(node_t*) * vec->capacity);
	if (vec->nodes == NULL) {
		// Memory alloc error
		return 1;
	}

	return 0;
}

int add_node(node_vec_t *vec, node_t *node) {
	if (vec->n >= vec->capacity)
		if (double_node_capacity(vec) == 1) {
			// Memory alloc error
			return 1;
		}
	
	vec->nodes[vec->n++] = node;

	return 0;
}

void free_node_vec(node_vec_t *vec) {
	free(vec->nodes);
	free(vec);
}
