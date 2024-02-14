#include "bit_operations.h"
#include "file_operations.h"
#include "../constants.h"
#include <stdlib.h>


// Returns file size in KB
long int get_file_size(const char *filepath) {
    FILE *f;
    long int file_size;

    f = fopen(filepath, "rb");
    if (f == NULL) {
        return -1;
    }

    fseek(f, 0L, SEEK_END);
    file_size = ftell(f);

    fclose(f);
    return file_size / 1024;
}

// Read one bit from file
int read_bit(int *shift, char **byte_binary, FILE *file) {
	int byte;

	if (*shift >= 8) {
		*shift = 0;

		if ((byte = getc(file)) == EOF) {
			return -1; // EOF
		}
		
		if (*byte_binary != NULL) {
			free(*byte_binary);
		}

		if ((*byte_binary = byte_to_binary(byte)) == NULL) {
			return -2; // Memory alloc error
		}
	}

	return (*byte_binary)[(*shift)++] - '0';
}

// Write one bit to file
void write_bit(int bit, int *shift, unsigned char *byte, FILE *file) {
	*byte |= (bit << (7 - *shift));
	(*shift)++;

	if (*shift >= BITS_IN_BYTE) {
		fwrite(byte, 1, sizeof(*byte), file);

		*byte = 0;
		*shift = 0;
	}
}

int is_next_byte_eof(FILE *file) {
    int c = fgetc(file);

    if (c == EOF) {
        return 1;
    } else {
        ungetc(c, file);
        return 0; 
    }
}

int is_file_empty(const char* filename) {
    FILE* file;
	long size;
	
	file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);

    return size == 0;
}
