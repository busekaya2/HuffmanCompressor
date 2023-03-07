#include "node.h"
#include <stdlib.h>

node_t * init_node(int sign, int freq, node_t *left, node_t *right){
	node_t *newnode = malloc(sizeof(node_t));
	
	if (newnode == NULL)
		// Błąd alokacji pamięci
		return NULL;	

	newnode->sign = sign;
	newnode->freq = freq;
	newnode->code = NULL;
	newnode->left = left;
	newnode->right = right;

	return newnode;
}

void free_node(node_t *node){	
	/* Zwalnianie pamięci kodu.
	 * Jeśli kod == NULL to znaczy, że pamięc nie była alokowana i nie ma potrzeby używać free. */
	if (node->code != NULL)
		free(node->code);

	free(node);
}
