#include <stdlib.h>
#include <stdio.h>
#include "node.h"


node_t *init_node(int value, int frequency, node_t *left_child, node_t *right_child, char* code)
{
	node_t *node = malloc(sizeof(node_t));

	node->value = value;
	node->frequency = frequency;
	node->left_child = left_child;
	node->right_child = right_child;
	node->code = code;

	return node;
}

