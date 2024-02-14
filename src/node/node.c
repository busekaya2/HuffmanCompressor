#include "node.h"
#include <stdlib.h>
#include <stdio.h>


node_t *init_node(unsigned char sign, unsigned int freq, node_t *left, node_t *right) {
	node_t *newnode;
	
	newnode = malloc(sizeof(node_t));
	if (newnode == NULL) {
		return NULL;	
	}

	newnode->sign = sign;
	newnode->freq = freq;
	newnode->left = left;
	newnode->right = right;

	return newnode;
}

void free_node(node_t *node) {
	if (node != NULL) {
		free(node);
	}
}
