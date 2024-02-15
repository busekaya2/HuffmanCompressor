#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "./vector/word_vector.h"
#include "./huffman/huffman_tree.h"
#include "./huffman/read_codes.h"
#include "./huffman/encode.h"
#include "./huffman/decode.h"
#include "./heap/heap_min.h"
#include "./utils/file_operations.h"
#include "./utils/path_handler.h"
#include "./error_codes.h"
#include "./constants.h"


void help() {
	printf("The program compresses files using the Huffman algorithm.\n");
	printf("During compression/decompression, a new file is created, containing the compressed data and dictionary.\n\n");
	printf("Syntax: ./compressor.exe [-f FILEPATH] [-d] [-s] [-h]\n\n");
	printf("Options:\n");
	printf("\t-f\tPath of the file to be compressed/decompressed\n");
	printf("\t-d\tSwitch to decopression (Program compresses by default)\n");
	printf("\t-s\tDisplay file size before and after compression/decompression\n");
	printf("\t-h\tDisplay help message\n\n");
	printf("The program can compress/decompress several files at once. In this case, the -f option should be placed before the path of each file.\n");
	printf("\tExample: ./compressor.exe -f file1 -f file2 -f file3\n");
}

void handle_error(int return_code, char *message, char *info) {
	switch (return_code) {
		case ERROR_MEMORY_ALLOC:
			if (info != NULL) {
				fprintf(stderr, "Memory allocation error while: %s for %s\n", message, info);
			}
			else {
				fprintf(stderr, "Memory allocation error while: %s\n", message);
			}
		break;

		case ERROR_INVALID_PARAMETERS:
			if (info != NULL) {
				fprintf(stderr, "Invalid parameter given: (%s) %s\n", info, message);
			}
			else {
				fprintf(stderr, "Invalid parameter given: %s\n", message);
			}
		break;

		// File specific error
		case ERROR_READING_FILE:
			fprintf(stderr, "Error reading file (%s): %s\n", info, message);
		break;

		case ERROR_WRITING_FILE:
			fprintf(stderr, "Error writing to file (%s): %s\n", info, message);
		break;

		case ERROR_INVALID_FILE_FORMAT:
			fprintf(stderr, "Invalid file format (%s): %s\n", message, info);
		break;
	}
}

void free_resources(heap_min_t *queue, word_vec_t *files, node_t *root, char *file_extension, char *filepath_no_extension, char *filepath_with_extension) {
    heap_min_free(queue);
    free_word_vec(files);
    free_tree(root);
    free(file_extension);
    free(filepath_no_extension);
    free(filepath_with_extension);
}

int main(int argc, char **argv) {
	int file_id, i;							
	int return_code;						

	// Getopt vars
	char opt;								
	int show_file_size = 0;					
	int decode_file = 0;					
	word_vec_t *files = NULL;						

	// Filepath handling
	char *filepath_no_extension = NULL;
	char *filepath_with_extension = NULL;

	// Compression
	heap_min_t *queue = NULL;						
	node_t *root = NULL;							
	huffman_code_t codes[BYTE_SIZE];
	char *file_extension = NULL;				

	// Decompression
	FILE *input_file = NULL;	
	FILE *output_file = NULL;
	char *original_file_extension = NULL;
	
	if ((files = init_word_vec(INITIAL_VECTOR_SIZE)) == NULL) {
		handle_error(ERROR_MEMORY_ALLOC, "creating filename vector", NULL);
		free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
		return ERROR_MEMORY_ALLOC;
	}

	// Handle invoke options using getopt
	while ((opt = getopt(argc, argv, "hdsf:")) != -1) {
		switch (opt) {
			case 'f':
				// Add path of file to vector 
				if (add_word(files, optarg) == 1) {
					handle_error(ERROR_MEMORY_ALLOC, "adding filename to vector", NULL);
					free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
					return ERROR_MEMORY_ALLOC;
				}
			break;

			case 'd':
				// Decompress option
				decode_file = 1;
			break;

			case 's':
				// Show filesize before and after compression
				show_file_size = 1;
			break;

			case 'h':
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				help();
			return EXIT_SUCCESS;

			case '?':
				handle_error(ERROR_INVALID_PARAMETERS, "Unknown parameter", optarg);
				help();
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
			return ERROR_INVALID_PARAMETERS;
		}
	}

	if (files->n < 1) {
		handle_error(ERROR_INVALID_PARAMETERS, "no file was given", NULL);
		help();
		free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
		return ERROR_INVALID_PARAMETERS;
	}

	if ((queue = init_heap_min(INITIAL_QUEUE_SIZE)) == NULL) {
		handle_error(ERROR_MEMORY_ALLOC, "initializing queue", NULL);
		free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
		return ERROR_MEMORY_ALLOC;
	}

	// Handling each given file
	for (file_id = 0; file_id < files->n; file_id++) {
		if (is_file_empty(files->words[file_id]) == 1) {
			handle_error(ERROR_INVALID_PARAMETERS, "given file is empty", files->words[file_id]);
			continue;
		}

		if (!decode_file) {
			// We're compressing

			// Create node queue
			if ((return_code = build_heap_queue(queue, files->words[file_id])) != EXIT_SUCCESS) {
				handle_error(return_code, "compressor: building queue", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return return_code;
			}

			// Build Huffman tree
			if ((root = make_tree(queue)) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "compressor: making huffman tree", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}
			if (get_tree_height(root) > MAX_CODE_LENGTH) {
				handle_error(ERROR_INVALID_FILE_FORMAT, "compressor: huffman code too long", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_INVALID_FILE_FORMAT;
			}

			// Read codes
			for (i = 0; i < BYTE_SIZE; i++) {
				codes[i].code = 0;
				codes[i].length = 0;
			}
			if ((return_code = read_codes(root, &codes)) != EXIT_SUCCESS) {
				handle_error(return_code, "compressor: reading codes from huffman tree", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return return_code;
			}

			// Change file extension to .huf
			if ((file_extension = get_file_extension(files->words[file_id])) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "compressor: saving original file extension", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}
			if ((filepath_no_extension = remove_file_extension(files->words[file_id])) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "compressor: removing old file extension", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}
			if ((filepath_with_extension = add_file_extension(filepath_no_extension, "huf")) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "compressor: changing file extension to .huf", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}

			// Encode file
			if ((return_code = encode(files->words[file_id], filepath_with_extension, file_extension, root, &codes)) != 0) {
				handle_error(return_code, "compressor: encoding file", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return return_code;
			}

			// Free for the next file
			free(file_extension);
			free_tree(root);
		}
		else {
			// We're decompressing

			//  Open input file
			if ((input_file = fopen(files->words[file_id], "rb")) == NULL) {
				handle_error(ERROR_READING_FILE, "decompressor: opening input file", files->words[file_id]);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_READING_FILE;
			}

			//  Decode file extension
			if ((original_file_extension = read_original_extension(input_file)) == NULL) {
				handle_error(ERROR_READING_FILE, "decompressor: reading file extension", files->words[file_id]);
				fclose(input_file);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_READING_FILE;
			}

			// Change file extension to original
			if ((filepath_no_extension = remove_file_extension(files->words[file_id])) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "decompressor: removing old file extension", files->words[file_id]);
				fclose(input_file);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}
			if ((filepath_with_extension = add_file_extension(filepath_no_extension, original_file_extension)) == NULL) {
				handle_error(ERROR_MEMORY_ALLOC, "decompressor: restoring original file extension", files->words[file_id]);
				fclose(input_file);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_MEMORY_ALLOC;
			}

			//  Open output file
			if ((output_file = fopen(filepath_with_extension, "wb")) == NULL) {
				handle_error(ERROR_WRITING_FILE, "decompressor: opening output file", files->words[file_id]);
				fclose(input_file);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return ERROR_WRITING_FILE;
			}

			// Decode file
			if ((return_code = decode(input_file, output_file)) != EXIT_SUCCESS) {
				handle_error(return_code, "decompressor: decoding file", files->words[file_id]);
				fclose(input_file);
				fclose(output_file);
				free_resources(queue, files, root, file_extension, filepath_no_extension, filepath_with_extension);
				return return_code;
			}

			// Free for the next file
			fclose(input_file);
			fclose(output_file);
			free(original_file_extension);
		}

		if (show_file_size) {
			printf("%s\n", files->words[file_id]);
			printf("\tSize before: %ld [KB]\n", get_file_size(files->words[file_id]));
			printf("\tSize after: %ld [KB]\n", get_file_size(filepath_with_extension));
		}

		free(filepath_no_extension);
		free(filepath_with_extension);
	}	
	
	// Free when job is done
	free_word_vec(files);
	heap_min_free(queue);
		
	return EXIT_SUCCESS;
}
