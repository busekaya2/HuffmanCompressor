#include "encode.h"
#include "../vector/node_vector.h"
#include <stdio.h>
#include <string.h>


char *find_code(int sign, node_vec_t *codes) {
	int i;

	for (i = 0; i < codes->n; i++) {
		if(codes->nodes[i]->sign == sign) {
			return codes->nodes[i]->code;
		}
	}
	
	return NULL;
}

void encode(FILE *in, FILE *out_file, char* file_ext, node_vec_t *codes) {
	char byte = 0;
	int shift = 0;
	char* code;	
	int i, c;	

	// Encoding file extension
	if (file_ext != NULL) {
		fprintf(out_file, "%s\n", file_ext);
	}
	else {
		fprintf(out_file, "\n");
	}

	// Encoding dictionary
	for (i = 0; i < codes->n; i++) {
		fprintf(out_file, "%d %s\n", codes->nodes[i]->sign, codes->nodes[i]->code);
	}

	// Encoding dictionary terminator: '\0'
	fwrite(&byte, 1, sizeof(byte), out_file);

	// Encoding data
	while ((c = getc(in)) != EOF) {
		code = find_code(c, codes);

		for (i = 0; i < strlen(code); i++) {
			if (shift > 7){
				fwrite(&byte, 1, sizeof(byte), out_file);
				byte = 0;
				shift = 0;
			}

			byte |= ((code[i] - '0') << (7 - shift));
			shift++;
		}
	}

	//  Encoding file terminator: -1 code
	code = find_code(-1, codes);		
	for (i = 0; i < strlen(code); i++) {
		if (shift > 7){
			fwrite(&byte, 1, sizeof(byte), out_file);
			byte = 0;
			shift = 0;
		}
		byte |= ((code[i] - '0') << (7 - shift));
		shift++;
	}
		
	// Fill last byte with zeros
	while (shift < 8) {
		byte |= (0 << (7 - shift));
		shift++;
	}

	fwrite(&byte, 1, sizeof(byte), out_file);
}
