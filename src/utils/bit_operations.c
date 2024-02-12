#include "bit_operations.h"
#include <stdlib.h>


// Converts byte(char) to binary string
char *byte_to_binary(char byte) {
	char* binary = (char*) malloc(9 * sizeof(char)); // 8 bits + '\0'
	if (binary == NULL) {
		// Memory alloc error
		return NULL;
	}

	for (int i = 7; i >= 0; i--) {
		binary[7 - i] = (byte & (1 << i)) ? '1' : '0';
	}

	binary[8] = '\0';

	return binary;
}
