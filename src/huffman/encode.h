#ifndef ENCODE_H
#define ENCODE_H


#include "../node/node.h"
#include "../huffman/huffman_tree.h"
#include "../constants.h"
#include <stdio.h>

int encode(char*, char*, char*, node_t*, huffman_code_t (*)[BYTE_SIZE]);
void encode_dictionary(int*, unsigned char*, FILE*, node_t*);
void encode_data(int*, unsigned char*, FILE*, FILE*, huffman_code_t (*)[BYTE_SIZE]);
void encode_padding(FILE*, int);


#endif
