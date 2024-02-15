#ifndef READ_CODES_H
#define READ_CODES_H


#include "../node/node.h"
#include "./huffman_tree.h"
#include "../constants.h"

int read_codes(node_t*, huffman_code_t(*)[BYTE_SIZE]);
void read_codes_recursive(node_t*, huffman_code_t*, huffman_code_t(*)[BYTE_SIZE]);


#endif