#include "decode.h"
#include "../utils/bit_operations.h"
#include "../utils/file_operations.h"
#include "../huffman/huffman_tree.h"
#include "../error_codes.h"
#include "../constants.h"
#include <string.h>
#include <stdlib.h>


char *read_original_extension(FILE *file) {
    int byte = 0;
    int length = 0;
	char *temp;
    int extension_length = INITIAL_EXTENSION_LENGTH;
	char *file_extension;

    file_extension = malloc(extension_length * sizeof(char));
    if (file_extension == NULL) {
        return NULL;
    }

    while ((byte = fgetc(file)) != '\n' && byte != EOF) {
        if (length + 1 >= extension_length) {
            extension_length *= 2;
            temp = realloc(file_extension, extension_length * sizeof(char));
            if (temp == NULL) {
                free(file_extension);
                return NULL; 
            }
            file_extension = temp;
        }

        (file_extension)[length++] = (char)byte;
    }

    file_extension[length] = '\0';
    return file_extension; 
}

int read_padding(int *shift, char **byte_binary, FILE *file) {
	int i;
	char padding_binary[PADDING_SIZE + 1];

	for (i = 0; i < PADDING_SIZE; i++) {
		if ((padding_binary[i] = read_bit(shift, byte_binary, file) + '0') < 0) {
			return padding_binary[i];
		}
	}
	padding_binary[PADDING_SIZE] = '\0';

	return binary_to_byte(padding_binary);
}

node_t *read_dictionary(int *shift, char **byte_binary, FILE *file) {
	int i;
	int bit;
	unsigned char sign;
	node_t *node;
	node_t *left_child;
	node_t *right_child;

	char sign_binary[BITS_IN_BYTE + 1];

	bit = read_bit(shift, byte_binary, file);

	if (bit == 0) { // Not a leaf
		left_child = read_dictionary(shift, byte_binary, file);
		right_child = read_dictionary(shift, byte_binary, file);

		node = init_node(0, 0, left_child, right_child);
	}
	else { // Leaf
		for (i = 0; i < BITS_IN_BYTE; i++) {
			sign_binary[i] = read_bit(shift, byte_binary, file) + '0';
		}
		sign_binary[BITS_IN_BYTE] = '\0';

		node = init_node(binary_to_byte(sign_binary), 0, NULL, NULL);
	}

	return node;
}

int decode_data(int *input_shift, char **byte_binary, FILE *input_file,
				int *output_shift, unsigned char *output_byte, FILE *output_file,
				node_t *root, int padding) {
	int bit;
	int i;
	node_t *head;

	if (root->left == NULL) {
		// Case when root is the only node

		while (is_next_byte_eof(input_file) == 0 || (is_next_byte_eof(input_file) == 1 && *input_shift < (BITS_IN_BYTE - padding))) {
			if ((bit = read_bit(input_shift, byte_binary, input_file)) < 0) {
				return bit;
			}

			fwrite(&(root->sign), 1, sizeof(char), output_file);
		}
	}

	while (is_next_byte_eof(input_file) == 0 || (is_next_byte_eof(input_file) == 1 && *input_shift < (BITS_IN_BYTE - padding))) {
		head = root;

		while (head->left != NULL) {
			bit = read_bit(input_shift, byte_binary, input_file);

			switch (bit) {
				case 0:
					head = head->left;
				break;

				case 1:
					head = head->right;
				break;

				default:
					return bit;
			}
		}

		fwrite(&(head->sign), 1, sizeof(char), output_file);
	}

	return EXIT_SUCCESS;
}

int handle_decode_error(int return_code) {
	switch (return_code) {
		case -1:
			return ERROR_INVALID_FILE_FORMAT;
		case -2:
			return ERROR_MEMORY_ALLOC;
	}

	return return_code;
}

int decode(FILE *input_file, FILE *output_file) {
	node_t *root;
	int padding;
	int return_code;

	char *byte_binary = NULL;
	int input_shift = BITS_IN_BYTE; // So function will open first byte
	
	int output_shift = 0;
	unsigned char output_byte = 0;

	if ((padding = read_padding(&input_shift, &byte_binary, input_file)) < 0) {
		return handle_decode_error(padding);
	}

	if ((root = read_dictionary(&input_shift, &byte_binary, input_file)) == NULL) {
		free(byte_binary);
		return ERROR_INVALID_FILE_FORMAT;
	}

	if ((return_code = decode_data(&input_shift, &byte_binary, input_file, &output_shift, &output_byte, output_file, root, padding)) != EXIT_SUCCESS) {
		free(byte_binary);
		free_tree(root);
		return handle_decode_error(return_code);
	}

	free(byte_binary);
	free_tree(root);

	return EXIT_SUCCESS;
}
