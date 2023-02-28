#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
	FILE *in;
	FILE *out_file;
	FILE *out_key;
	node_t *root;
	node_t *temp_node;
	char* end_code;
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
		
		in = fopen(file_name, "rb");
	
		if (in == NULL)
			return -1;
	
		// Wyznaczanie częstości bytów pliku
		while ((byte = fgetc(in)) != EOF)
		{
			v[byte]++;
			n++;
		}

		fclose(in);

		// Odczytywanie rozszerzenia pliku
		file_name_n = strlen(file_name);
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
		
		// Tworzenie specjalnego kodu konczacego plik wynikowy
		printf("codes[0]: %s\n", codes->nodes[0]->code);
		end_code = malloc(sizeof(char) * (strlen(codes->nodes[0]->code) + 2));
		strcpy(end_code, codes->nodes[0]->code);
		end_code[strlen(codes->nodes[0]->code)] = '0';
		end_code[strlen(codes->nodes[0]->code) + 1] = '\0';

		printf("end: %s\n", end_code);

		// Kodowanie do pliku
		in = fopen(file_name, "rb");
		out_file = fopen("out.huf", "wb");
		out_key = fopen("out.key", "wb");

		encode(in, out_file, out_key, file_extension, codes, end_code);

		fclose(in);
		fclose(out_file);
		fclose(out_key);

		// Zwalnianie pamięci
		free(end_code);
		free_word_array(files);
		free_tree(root);
		free_vector(codes);
		free(nodes->nodes);
		free(nodes);
	}

	return 0;
}
