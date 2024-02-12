#include "huffman.h"
#include "../vector/node_vector.h"
#include "../heap/heap_min.h"
#include "../node/node.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>


int build_heap_content(FILE *input, heap_min_t *nodes) {
	int i;
	int byte;
	int *freq = malloc(sizeof(int) * BYTE_SIZE); 
	
	if (freq == NULL) {
		return 1;
	}

	for (i = 0; i < BYTE_SIZE; i++) {
		freq[i] = 0;
	}
	
	while ((byte = fgetc(input)) != EOF) {
		freq[byte]++;

		if (freq[byte] >= INT_MAX) {
            for (i = 0; i < BYTE_SIZE; i++) {
				freq[i] /= 2;
			}
		}
	}

	for (i = 0; i < BYTE_SIZE; i++) {
		if (freq[i] != 0) {
			if (heap_min_insert(nodes, init_node(i, freq[i], NULL, NULL)) != 0) {
				free(freq);
				return 1;
			}
		}
	}

	return 0;
}

node_t * make_tree(heap_min_t *heap_min) {
	node_t *node_a;
	node_t *node_b;
	node_t *new_node;
	
	// Connect nodes until there's only one
	while (heap_min->n > 1) {
		node_a = heap_min_remove(heap_min);
		node_b = heap_min_remove(heap_min);

		if ((new_node = init_node(0, node_a->freq + node_b->freq, node_a, node_b)) == NULL) {
			// Memory alloc error
			return NULL;
		}

		if ((heap_min_insert(heap_min, new_node)) != 0) {
			// Memory alloc error
			free(new_node);
			return NULL;
		}
	}

	// The last node is the root 
	return heap_min_remove(heap_min);
}

void read_codes(node_t *root, node_vec_t *codes, char *temp_code) {
	char *tmp;

	if (root == NULL) {
		return;
	}

	if (root->left != NULL) {
		// The left child is not a leaf, go deeper
		strcat(temp_code, "0");
		read_codes(root->left, codes, temp_code);
	}
	if (root->right != NULL) {
		// The right child is not a leaf, go deeper
		strcat(temp_code, "1");
		read_codes(root->right, codes, temp_code);
	}

	// This node is a leaf, let's save its code
	if (root->left == NULL && root->right == NULL) {
		if ((tmp = malloc(sizeof(char) * (strlen(temp_code) + 1))) == NULL) {
			return;
		}

		strcpy(tmp, temp_code);
		add_node(codes, init_node(root->sign, root->freq, NULL, NULL));
		codes->nodes[codes->n - 1]->code = tmp;
	}	

	// Remove last code's bit when returning to previous node
	if (strlen(temp_code) > 0)
		temp_code[strlen(temp_code) - 1] = '\0';
}

void free_tree(node_t *root) {
	if (root == NULL) {
		return;
	}

	free_tree(root->left);		
	free_tree(root->right);
	free_node(root);
}
