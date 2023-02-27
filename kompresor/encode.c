#include "encode.h"
#include "huffman_tree.h"
#include "vector.h"


void encode(FILE *file, char *file_ext, char *output, int print_codes, node_t *head)
{
	char temp_str[256] = "\0";
	vector_t *codes = init_vector();

	read_codes(head, temp_str, 0, codes);

	for (int i = 0; i < codes->n; i++)
	{
		printf("%d %s\n", codes->nodes[i]->value, codes->nodes[i]->code);
	}

	free_vector(codes);
}
