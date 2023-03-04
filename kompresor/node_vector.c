#include "node_vector.h"
#include "node.h"
#include <stdlib.h> 


node_vec_t * init_node_vec(int capacity)
{
	node_vec_t *newvec = malloc(sizeof(node_vec_t));
	if(newvec == NULL)
		return NULL;
	
	newvec->nodes = malloc(sizeof(node_vec_t) * capacity);
	if(newvec->nodes == NULL)
		return NULL;

	newvec->capacity = capacity;
	newvec->n = 0;
	
	return newvec;
}

int double_node_capacity(node_vec_t* vec)
{
	vec->capacity *= 2;
	vec->nodes = realloc(vec->nodes, sizeof(node_t) * vec->capacity);
	if(vec == NULL)
		return 1;

	return 0;
}

int add_node(node_vec_t *vec, node_t *node)
{
	if(vec->n >= vec->capacity)
		if(double_node_capacity(vec) == 1)
			return 1;
	
	vec->nodes[vec->n++] = node;

	return 0;
}


/* UWAGA - Funkcja nie zwalnia zawartości wektora, jedynie strukturę.
 * Zawartość należy zwolnić ręcznie */
void free_node_vec(node_vec_t *vec)
{
	free(vec->nodes);
	free(vec);
}
