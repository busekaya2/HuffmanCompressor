#include "read_codes.h"
#include "../error_codes.h"
#include "../constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int read_codes(node_t *root, hashmap_t *map) {
	char temp_code[BYTE_SIZE];
	char *tmp;

	temp_code[0] = '\0';

	if (root->left == NULL) {
		// Case when root is the only leaf

		strcat(temp_code, "0");
		if ((tmp = malloc(sizeof(char) * (strlen(temp_code) + 1))) == NULL) {
			return ERROR_MEMORY_ALLOC;
		}

		strcpy(tmp, temp_code);
		hashmap_insert(map, root->sign, tmp);
		return EXIT_SUCCESS;
	}

	read_codes_recursive(root, temp_code, map);

	return EXIT_SUCCESS;
}


void read_codes_recursive(node_t *head, char *temp_code, hashmap_t *map) {
	char *tmp;

	if (head == NULL) {
		return;
	}

	if (head->left != NULL) {
		// This node is not a leaf, go deeper

		read_codes_recursive(head->left, strcat(temp_code, "0"), map);
		read_codes_recursive(head->right, strcat(temp_code, "1"), map);
	}
	else {
		// This node is a leaf, let's save its code

		if ((tmp = malloc(sizeof(char) * (strlen(temp_code) + 1))) == NULL) {
			return;
		}

		strcpy(tmp, temp_code);
		hashmap_insert(map, head->sign, tmp);
	}

	// Remove last code's bit when returning to previous node
	if (strlen(temp_code) > 0) {
		temp_code[strlen(temp_code) - 1] = '\0';
    }
}
