#include <stdlib.h>
#include "vector.h"


vector_t *init_vector()
{
	vector_t *v = malloc(sizeof(vector_t));
	
	if (v == NULL)
		return NULL;

	v->n = 0;
	v->memory = 8;
	v->nodes = malloc(sizeof(node_t) * v->memory);
	
	if (v->nodes == NULL)
		return NULL;

	return v;
}


int increase_memory(vector_t *v)
{
	v->memory *= 2;
	v->nodes = realloc(v->nodes, sizeof(node_t) * v->memory);
	
	if (v == NULL)
		return 1;

	return 0;
}


int add_element(vector_t *v, node_t *node)
{
	if (v->n + 1 > v->memory)
		if (increase_memory(v) == 1)
			return 1;

	v->nodes[(v->n)++] = node;

	return 0;
}

void free_vector(vector_t *v)
{
	int i;

	for(i = 0; i < v->n; i++)
	{
		if ((v->nodes[i])->code[0] != '\0')
		{
			free((v->nodes[i])->code);
		}
		free(v->nodes[i]);
	}
	free(v->nodes);
	free(v);
}

