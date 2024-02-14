#include "encode.h"
#include "../utils/bit_operations.h"
#include "../utils/file_operations.h"
#include "../error_codes.h"
#include "..//constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int encode(char *input_filename, char *output_filename, char* file_ext, node_t *root, hashmap_t *codes_map) {
	unsigned char byte = 0;
	int shift = 0;
	char *code;	
	int i, padding;
	FILE *input_file;
	FILE *output_file;

	if ((input_file = fopen(input_filename, "rb")) == NULL) {
		return ERROR_READING_FILE;
	}

	if ((output_file = fopen(output_filename, "wb")) == NULL) {
		fclose(input_file);
		return ERROR_WRITING_FILE;
	}

	// Encode file extension
	if (file_ext != NULL) {
		fprintf(output_file, "%s\n", file_ext);
	}
	else {
		fprintf(output_file, "\n");
	}

	// Leave empty space for padding
	for (i = 0; i < PADDING_SIZE; i++) {
		write_bit(0, &shift, &byte, output_file);
	}

	encode_dictionary(&shift, &byte, output_file, root);
	encode_data(&shift, &byte, input_file, output_file, codes_map);

	// Fill last byte with zeros and re-open file
	padding = BITS_IN_BYTE - shift;

	while (shift > 0) { 
		write_bit(0, &shift, &byte, output_file);
	}
	
	fclose(output_file);
	if ((output_file = fopen(output_filename, "rb+")) == NULL) {
		fclose(input_file);
		return ERROR_WRITING_FILE;
	}

	encode_padding(output_file, padding);

	fclose(input_file);
	fclose(output_file);

	return EXIT_SUCCESS;
}

void encode_dictionary(int *shift, unsigned char *byte, FILE *file, node_t *head) {
	int i;
	char *sign_binary;

	if (head->left == NULL) {
		write_bit(1, shift, byte, file);

		sign_binary = byte_to_binary(head->sign);
		for (i = 0; i < BITS_IN_BYTE; i++) {
			write_bit(sign_binary[i] - '0', shift, byte, file);
		}
		free(sign_binary);
	}
	else {
		write_bit(0, shift, byte, file);

		encode_dictionary(shift, byte, file, head->left);
		encode_dictionary(shift, byte, file, head->right);
	}
}

void encode_data(int *shift, unsigned char *byte, FILE *input_file, FILE *output_file, hashmap_t *codes_map) {
	int i;
	int input_byte;
	char *code;

	while ((input_byte = getc(input_file)) != EOF) {
		code = hashmap_get(codes_map, input_byte);

		for (i = 0; i < strlen(code); i++) {
			write_bit(code[i] - '0', shift, byte, output_file);
		}
	}
}

void encode_padding(FILE *output_file, int padding) {
    int c;

    while ((c = fgetc(output_file)) != EOF) {
        if (c == '\n') {
            long pos = ftell(output_file);
            int nextByte = fgetc(output_file);
            
			if (nextByte != EOF) {
                nextByte |= padding << (BITS_IN_BYTE - PADDING_SIZE);

                fseek(output_file, pos, SEEK_SET);
                fputc(nextByte, output_file);
                fflush(output_file);
            }

            break;
        }
    }
}
