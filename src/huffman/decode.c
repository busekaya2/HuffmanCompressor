#include "decode.h"
#include "../node/node.h"
#include "../utils/bit_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int decode(FILE *in, FILE *out, node_t *root) {
	int i, c;
	char* bin;		
	node_t *head = root;	
	
	// "Eat" dictionary
	while ((c = fgetc(in)) != '\0') {}

	while ((c = fgetc(in)) != EOF) {
		bin = byte_to_binary(c);
		
		if (bin == NULL) {
			// Memory alloc error
			return 1;
		}

		for (i = 0; i < strlen(bin); i++) {
			// Go deeper
			if (bin[i] == '0') {
				head = head->left;
			}
			
			if (bin[i] == '1') {
				head = head->right;
			}

			if (head->left == NULL && head->right == NULL) {
				if (head->sign == -1) {
					// File terminator found
					free(bin);
					return 0;
				}

				// Save byte to file and go back to the tree root 
				fwrite(&(head->sign), 1, sizeof(char), out);
				head = root;
		
			}
		}
		free(bin);
	}

	// File terminator not found - broken file format
	return 2;
}
