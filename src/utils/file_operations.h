#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H


#include <stdio.h>

long int get_file_size(const char*);
int read_bit(int*, char**, FILE*);
void write_bit(int, int*, unsigned char*, FILE*);
int is_next_byte_eof(FILE*);
int is_file_empty(const char*);


#endif
