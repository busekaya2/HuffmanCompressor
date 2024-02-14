#include "huffman_tree.h"
#include "../error_codes.h"
#include "../constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


int build_heap_queue(heap_min_t *queue, char *input_filename) {
	int i;
	int byte;
	int freq[BYTE_SIZE] = {0}; 
	FILE *input;

	if ((input = fopen(input_filename, "rb")) == NULL) {
		return ERROR_READING_FILE;
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
			if ((heap_min_insert(queue, init_node(i, freq[i], NULL, NULL))) != EXIT_SUCCESS) {
				fclose(input);
				return ERROR_MEMORY_ALLOC;
			}
		}
	}

	fclose(input);
	return EXIT_SUCCESS;
}

node_t *make_tree(heap_min_t *heap_min) {
	node_t *node_a;
	node_t *node_b;
	node_t *new_node;
	
	// Connect nodes until there's only one left
	while (heap_min->n > 1) {
		node_a = heap_min_remove(heap_min);
		node_b = heap_min_remove(heap_min);

		if ((new_node = init_node(0, node_a->freq + node_b->freq, node_a, node_b)) == NULL) {
			return NULL;
		}

		if ((heap_min_insert(heap_min, new_node)) != EXIT_SUCCESS) {
			free(new_node);
			return NULL;
		}
	}

	// Return root 
	return heap_min_remove(heap_min);
}

void free_tree(node_t *root) {
	if (root == NULL) {
		return;
	}

	free_tree(root->left);		
	free_tree(root->right);
	free_node(root);
}
