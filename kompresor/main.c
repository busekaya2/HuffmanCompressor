#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "huffman_tree.h"
#include "vector.h"
#include "encode.h"


int main()
{
	char file_name[256] = "test.txt";
	char file_extension[50] = "";
	int byte, n, i, j;
	int file_name_n = 0;
	int ext_n = 0;
	int v[256] = {0};
	FILE *f;
	vector_t *nodes = init_vector();

	f = fopen(file_name, "rb");

	if (f == NULL)
		return -1;

	for (i = 0; file_name[i] != '\0'; i++)
    		file_name_n++;

	for (i = file_name_n - 1; i > 0; i--)
	{
		if (file_name[i] == '.')
		{
			for (j = i + 1; j < file_name_n; j++)
				file_extension[ext_n++] = file_name[j];
			file_extension[ext_n] = '\0';
			break;
		}
	}
	printf("%s\n", file_extension);

	while ((byte = fgetc(f)) != EOF)
	{
		v[byte]++;
		n++;
	}

	fclose(f);

	node_t *head = make_tree(v, nodes, 256);

	encode(stdout, file_name, file_extension, 0, head);
	
	free_tree(head);
	free(nodes->nodes);
	free(nodes);

	return 0;
}
