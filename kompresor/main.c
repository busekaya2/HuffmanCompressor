#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "huffman_tree.h"
#include "vector.h"
#include "encode.h"
#include "word_array.h"


int main(int argc, char** argv)
{
	char *file_name;
	char file_extension[50];
	int byte, n, i, j, file_n;
	int file_name_n;
	int ext_n;
	int v[256];
	FILE *f;
	node_t *root;
	vector_t *nodes;
	char temp_str[256];
	vector_t *codes;
	
	word_array_t *files = init_word_array(2);
	char opt;
	char *output_directory;
	int show_size = 0;
	int show_codes = 0;

	while ((opt = getopt(argc, argv, "o:f:")) != -1)
	{
    		switch (opt)
	      	{
      			case 'o':
        			output_directory = optarg;
	        		break;
	      		case 'f':
        			add_word(files, optarg);
        			break;
  	    		case '?':
				break;
		}
	}

	for (file_n = 0; file_n < files->n; file_n++)
	{
		// Ustawianie domyślnych wartości zmiennych dla kolejnego pliku
		file_name = files->words[file_n];
		file_name_n = 0;
		ext_n = 0;
		for (i = 0; i < 256; i++)
			v[i] = 0;
		nodes = init_vector();
		
		f = fopen(file_name, "rb");
	
		if (f == NULL)
			return -1;
	
		// Odczytywanie długości nazwy pliku
		for (i = 0; file_name[i] != '\0'; i++)
    			file_name_n++;
	
		// Odczytywanie rozszerzenia pliku
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
	
		// Wyznaczanie częstości bytów pliku
		while ((byte = fgetc(f)) != EOF)
		{
			v[byte]++;
			n++;
		}

		fclose(f);

		// Tworzenie drzewa binarnego Huffmana
		root = make_tree(v, nodes, 256);
	
		// Czytanie drzewa i zapisywanie kodów znaków
		temp_str[0] = '\0';
		codes = init_vector();

		read_codes(root, temp_str, 0, codes);

		for (int i = 0; i < codes->n; i++)
		{
			printf("%d %s\n", codes->nodes[i]->value, codes->nodes[i]->code);
		}
	
		// Kodowanie do pliku
		// encode(stdout, file_name, file_extension, 0, head);

		free_vector(codes);
		free_tree(root);
		free(nodes->nodes);
		free(nodes);
	}

	return 0;
}
