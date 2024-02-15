#include "read_codes.h"
#include "../error_codes.h"
#include "../constants.h"
#include <stdlib.h>
#include <stdio.h>


int read_codes(node_t *root, huffman_code_t (*codes)[BYTE_SIZE]) {
	if (root == NULL) {
		return ERROR_MEMORY_ALLOC;
	}

	huffman_code_t temp_code;
	temp_code.code = 0;
	temp_code.length = 0;

	if (root->left == NULL) {
		// Case when root is the only leaf

		add_bit(&((*codes)[root->sign]), 0);

		return EXIT_SUCCESS;
	}

	read_codes_recursive(root, &temp_code, codes);

	return EXIT_SUCCESS;
}

void read_codes_recursive(node_t *head, huffman_code_t *temp_code, huffman_code_t (*codes)[BYTE_SIZE]) {
	if (head->left != NULL) {
		// This node is not a leaf, go deeper

		add_bit(temp_code, 0);
		read_codes_recursive(head->left, temp_code, codes);

		add_bit(temp_code, 1);
		read_codes_recursive(head->right, temp_code, codes);
	}
	else {
		// This node is a leaf, let's save its code
		
		(*codes)[head->sign].code = temp_code->code;
		(*codes)[head->sign].length = temp_code->length;
	}

	// Remove last code's bit when returning to previous node
	clear_bit(temp_code);
}
