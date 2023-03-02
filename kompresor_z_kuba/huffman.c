#include <stdlib.h>
#include <stdio.h>
#include "node_vector.h"
#include "node.h"

int node_cmp(const void *a, const void *b)
{
	return (*(node_t**)a)->freq - (*(node_t**)b)->freq;
}

node_t * make_tree(node_vec_t *nodes)
{
	int i;
	node_t *temp;

	qsort(nodes->nodes, nodes->n, sizeof(node_t*), node_cmp);
	
	while (nodes->n > 1)
	{
		nodes->nodes[0] = init_node(0, nodes->nodes[0]->freq + nodes->nodes[1]->freq, nodes->nodes[0], nodes->nodes[1]);

		for (i = 1; i < nodes->n - 1; i++)
            		nodes->nodes[i] = nodes->nodes[i + 1];
		nodes->n--;
		
		for (i = 0; i < nodes->n - 1 && (nodes->nodes[i])->freq > (nodes->nodes[i + 1])->freq; i++)
		{
            		temp = nodes->nodes[i];
			nodes->nodes[i] = nodes->nodes[i + 1];
			nodes->nodes[i + 1] = temp;
		}
	}

	return nodes->nodes[0];
}

void read_codes(node_t *head, node_vec_t *codes)
{
	
}
