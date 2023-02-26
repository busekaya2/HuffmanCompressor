#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "huffman_tree.h"
#include "vector.h"


// Porównywanie liczb dla sortowania rosnącego
/*int intcmp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}*/


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

	//qsort(v, 256, sizeof(int), intcmp);

	/*
	for (int i = 0; i < 256; i++)
	{
		printf("%d\n", v[i]);
	}
	*/

	node_t *head = make_tree(v, nodes, 256);

	printf("%d\n", head->frequency);

	read_codes(head, temp_str, 0);

	return 0;
}
