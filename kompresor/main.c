#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "huffman_tree.h"
#include "vector.h"


int main()
{
	char file[50] = "test.txt";
	int b, n;
	int v[256] = {0};
	char temp_str[50] = "\0";
	vector_t *nodes = init_vector();
	FILE *f = fopen(file, "rb");

	if (f == NULL)
		return -1;

	while ((b = fgetc(f)) != EOF)
	{
		v[b]++;
		n++;
	}

	fclose(f);

	node_t *head = make_tree(v, nodes, 256);

	read_codes(head, temp_str, 0);

	free_vector(nodes);

	return 0;
}
