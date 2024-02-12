#include "file_operations.h"


// Returns file size in KB
long int get_file_size(FILE *f) {
	fseek(f, 0L, SEEK_END);
	long int file_size = ftell(f);

	return file_size / 1024;
}
