#include "node.h"
#include <stdlib.h>
#include <stdio.h>


node_t *init_node(int sign, int freq, node_t *left, node_t *right) {
	node_t *newnode = malloc(sizeof(node_t));
	
	if (newnode == NULL) {
		// Memory alloc error
		return NULL;	
	}

	newnode->sign = sign;
	newnode->freq = freq;
	newnode->code = NULL;
	newnode->left = left;
	newnode->right = right;

	return newnode;
}

void free_node(node_t *node) {	
	if (node->code != NULL) {
		free(node->code);
	}

	free(node);
}
