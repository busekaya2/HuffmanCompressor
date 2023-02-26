#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "vector.h"


int intcmp(const void *a, const void *b)
{
	return (*(node_t**)a)->frequency - (*(node_t**)b)->frequency;
}


node_t *make_tree(int *values, vector_t *nodes, int n)
{
	int i;
	node_t *parent = NULL;
	node_t *temp_node = NULL;

	for (i = 0; i < n; i++)
	{
		if (values[i] == 0)
			continue;

		add_element(nodes, init_node(i, values[i], NULL, NULL));
	}

	qsort(nodes->nodes, nodes->n, sizeof(node_t*), intcmp);

	while (nodes->n > 1)
	{
		nodes->nodes[0] = init_node(0, nodes->nodes[0]->frequency + nodes->nodes[1]->frequency, nodes->nodes[0], nodes->nodes[1]);
		//nodes->nodes[0] = parent;

		for (i = 1; i < nodes->n - 1; i++)
            		nodes->nodes[i] = nodes->nodes[i + 1];
		nodes->n--;
		
		for (i = 0; i < nodes->n - 1 && (nodes->nodes[i])->frequency > (nodes->nodes[i + 1])->frequency; i++)
		{
            		temp_node = nodes->nodes[i];
			nodes->nodes[i] = nodes->nodes[i + 1];
			nodes->nodes[i + 1] = temp_node;
		}
	}

	return nodes->nodes[0];
}


void read_codes(node_t *head, char* str, int level)
{
	/*
	if (head == NULL)
	{
        	return;
    	}
	*/

	if (head->left_child == NULL && head->right_child == NULL)
	{
		printf("%d %s\n", head->value, str);
		return;
	}

	if (head->left_child != NULL)
	{
		str[level] = '0';
		str[level + 1] = '\0';
		read_codes(head->left_child, str, level + 1);
	}
	
	if (head->right_child != NULL)
	{
		str[level] = '1';
		str[level + 1] = '\0';
		read_codes(head->right_child, str, level + 1);
	}
}


