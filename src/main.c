#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "./vector/word_vector.h"
#include "./vector/node_vector.h"
#include "./huffman/huffman.h"
#include "./huffman/encode.h"
#include "./huffman/decode.h"
#include "./heap/heap_min.h"
#include "./utils/file_operations.h"


#define BYTE_SIZE 256
#define DEFAULT_DATA_SIZE 8

void help() {
	printf("The program compresses files using the Huffman algorithm.\n");
	printf("During compression, a file with the extension .huf is created, containing the compressed data and the key for its decompression.\n\n");
	printf("Syntax: ./compressor.exe [-f FILE] [-s] [-h] [-d]\n\n");
	printf("Options:\n");
	printf("\t-f\tPath of the file to be compressed\n");
	printf("\t-s\tDisplay file size before and after compression\n");
	printf("\t-h\tDisplay help window\n");
	printf("\t-d\tImmediately decompress the compressed file to check the correctness of the compression\n\n");
	printf("The program can compress several files at once. In this case, the -f option should be placed before the path of each file.\n");
	printf("\tExample: ./compressor.exe -f file1 -f file2 -f file3\n");
}

int main(int argc, char **argv) {
	int i, j;								// Variables for iterating

	// getopt variables
	char opt;								// Variable determining getopt option
	word_vec_t *files = init_word_vec(2); 	// Vector storing names of files to be compressed
	int show_file_size = 0;					// Option to display size before and after compression
	int decode_file = 0;					// Option to decompress compressed file

	// Reading files for compression
	FILE *in; 								// Input file
	int freq[BYTE_SIZE]; 					// Frequency of byte occurrences in the file
	int byte; 								// Stores values <0; 255>

	// Creating the tree
	heap_min_t *nodes; 						// Vector of nodes (without codes)
	node_t *root; 							// Root of the tree

	// Encoding to file
	node_vec_t *codes; 						// Stores nodes with codes
	char temp_code[BYTE_SIZE]; 				// Temporary character string needed for generating codes
	char *file_ext; 						// Original file extension
	int file_name_n;						// Length of the original file name without the extension
	char *file_name;						// Original file name without the extension
	char *out_file_name;					// Name of the compressed output file
	char *out_decoded_name;					// Name of the decompressed file (optional execution)
	FILE *out_file;							// Output compressed file
	FILE *out_decoded;						// Output file after decompressing the compressed file (optional execution)
	
	// Handle invoke options using getopt
	while ((opt = getopt(argc, argv, "hdsf:")) != -1) {
		switch (opt) {
			case 'f':
				// Add path of file to vector 
				if (add_word(files, optarg) == 1) {
					fprintf(stderr, "%s: Filename memory alloc error: %s\n", argv[0], files->words[j]);
					return 3;
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
				help();
			return 0;

			case '?':
				help();
			return 4;
		}
	}

	if (files->n < 1) {
		fprintf(stderr, "%s: No file given.\n\n", argv[0]);
		help();
		return 1;
	}

	// Compressing each given file
	for (j = 0; j < files->n; j++){	
		if ((in = fopen(files->words[j], "rb")) == NULL) {
			fprintf(stderr, "%s: Error reading file: %s\n", argv[0], files->words[j]);
			return 1;
		}
		
		for (i = 0; i < BYTE_SIZE; i++) {
			freq[i] = 0;
		}
		
		while ((byte = fgetc(in)) != EOF) {
			freq[byte]++;
		}
		
		fclose(in);

		// Initialize node heap
		if ((nodes = init_heap_min(DEFAULT_DATA_SIZE)) == NULL) {
			fprintf(stderr, "%s: Error allocating heap: %s\n", argv[0], files->words[j]);
			return 3;
		}

		for (i = 0; i < BYTE_SIZE; i++) {
			if (freq[i] != 0) {
				if (heap_min_insert(nodes, init_node(i, freq[i], NULL, NULL)) != 0) {
					fprintf(stderr, "%s: Error allocating node: %s\n", argv[0], files->words[j]);
					return 3;
				}
			}
		}
		
		// Add file termination node (-1)
		if (heap_min_insert(nodes, init_node(-1, 1, NULL, NULL)) == 1) {
			fprintf(stderr, "%s: Error allocating file termination node: %s\n", argv[0], files->words[j]);
			return 3;
		}

		// Create Huffman tree
		if ((root = make_tree(nodes)) == NULL) {
			fprintf(stderr, "%s: Error allocating huffman tree: %s\n", argv[0], files->words[j]);
			return 3;
		}

		// Read codes from tree
		if ((codes = init_node_vec(DEFAULT_DATA_SIZE)) == NULL) {
			fprintf(stderr, "%s: Error allocating codes vector: %s\n", argv[0], files->words[j]);
			return 3;
		}

		temp_code[0] = '\0';
		read_codes(root, codes, temp_code);

		for (i = 0; i < codes->n; i++) {
			if (codes->nodes[i]->code == NULL) {
				fprintf(stderr, "%s: Error allocating code string: %s\n", argv[0], files->words[j]);
				return 3;
			}
		}

		// Read original extension
		file_ext = strrchr(files->words[j], '.');
		if (file_ext != NULL) {
			file_ext++;
			if ((file_ext - 1) - strrchr(files->words[j], '/') < 0) {
				file_ext = NULL;
			}
		}
			
		// Count original filename length (without extension)
		file_name_n;
		if (file_ext != NULL)
			file_name_n = strlen(files->words[j]) - strlen(file_ext) - 1;
		else
			file_name_n = strlen(files->words[j]);
		
		// Save filename (without extension)
		file_name = malloc(sizeof(char) * (file_name_n + 1));
		if (file_name == NULL){
			fprintf(stderr, "%s: Error allocating filename string: %s\n", argv[0], files->words[j]);
			return 3;
		}

		for (i = 0; i < file_name_n; i++)
			file_name[i] = files->words[j][i];
		file_name[i] = '\0';
		
		// Create output filenames
		out_file_name = malloc(sizeof(char) * (file_name_n + 5));
		if (out_file_name == NULL){
			fprintf(stderr, "%s: Error allocating output filename string: %s\n", argv[0], files->words[j]);
			return 3;
		}
		
		strcpy(out_file_name, file_name);
		strcat(out_file_name, ".huf\0");
				
		// Encode file
		in = fopen(files->words[j], "rb");
		if (in == NULL){
			fprintf(stderr, "%s: Error reading file: %s\n", argv[0], files->words[j]);
			return 1;
		}

		out_file = fopen(out_file_name, "wb");
		if (out_file == NULL){
			fprintf(stderr, "%s: No privilages to: %s\n", argv[0], files->words[j]);
			return 2;
		}

		encode(in, out_file, file_ext, codes);
	
		// Show filesize before and after compression (invoke option)
		if (show_file_size){
			printf("%s\n", files->words[j]);
			printf("\tSize before: %ld [KB]\n", get_file_size(in));
			printf("\tSize after: %ld [KB]\n", get_file_size(out_file));
		}

		fclose(in);
		fclose(out_file);

		// Decode file (invoke option)
		if (decode_file){
			if (file_ext != NULL)
				out_decoded_name = malloc(sizeof(char) * (file_name_n + 10 + strlen(file_ext)));
			else
				out_decoded_name = malloc(sizeof(char) * (file_name_n + 9));
			
			if (out_decoded_name == NULL){
				fprintf(stderr, "%s: Error allocating decompressed output filename string: %s\n", argv[0], files->words[j]);
				return 3;
			}

			strcpy(out_decoded_name, file_name);
			strcat(out_decoded_name, "_decoded");
			
			if (file_ext != NULL){
				strcat(out_decoded_name, ".");
				strcat(out_decoded_name, file_ext);
			}
			strcat(out_decoded_name, "\0");
		
			out_decoded = fopen(out_decoded_name, "wb");
			if (out_decoded == NULL){
				fprintf(stderr, "%s: No privilages to: %s\n", argv[0], files->words[j]);
				return 2;
			}

			out_file = fopen(out_file_name, "rb");
			if (out_file == NULL){
				fprintf(stderr, "%s: Error reading file: %s\n", argv[0], files->words[j]);
				return 1;
			}

			decode(out_file, out_decoded, root);
		
			free(out_decoded_name);

			fclose(out_decoded);
			fclose(out_file);
		}

		// Free memory for the next file
		free(file_name);
		free(out_file_name);
		heap_min_free(nodes);
		
		for (i = 0; i < codes->n; i++)
			free_node(codes->nodes[i]);
		free_node_vec(codes);
		
		free_tree(root);
	}	
	
	free_word_vec(files);
		
	return 0;
}
