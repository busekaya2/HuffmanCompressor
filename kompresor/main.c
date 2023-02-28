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
	char *input_path;
	char *file_ext;
	char *file_name;
	int file_ext_n;
	int byte, n, i, j, file_n;
	int input_path_n;
	int v[256];
	FILE *in;
	FILE *out_file;
	FILE *out_key;
	char *out_file_path;
	char *out_key_path;
	node_t *root;
	node_t *temp_node;
	char* end_code;
	vector_t *nodes;
	char temp_str[256];
	vector_t *codes;
	
	word_array_t *files = init_word_array(2);
	char opt;
	char *out_dir = NULL;
	int show_size = 0;
	int show_codes = 0;

	while ((opt = getopt(argc, argv, "o:f:")) != -1)
	{
    		switch (opt)
	      	{
      			case 'o':
        			out_dir = optarg;
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
		input_path = files->words[file_n];
		input_path_n = 0;
		for (i = 0; i < 256; i++)
			v[i] = 0;
		nodes = init_vector();
		
		in = fopen(input_path, "rb");
	
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
		file_ext = strrchr(input_path, '.');
		if (file_ext == NULL)
			file_ext_n = -1;	
		else
		{
			file_ext_n = strlen(file_ext) - 1;
			file_ext++;
		}
		
		// Odczytywanie sciezki pliku bez rozszerzenia
		if (out_dir == NULL) // Gdy użytkownik podał ścieżkę 
		{
			j = strlen(input_path) - file_ext_n - 1;
			file_name = malloc(sizeof(char) * j);
			for (i = 0; i < j; i++)
				file_name[i] = input_path[i];
			file_name[j] = '\0';
		}
		else // Gdy nie podał (działa *jakoś*)
		{
			j = strlen(out_dir) + strlen(input_path) - file_ext_n;
			file_name = malloc(sizeof(char) * j);
			strcpy(file_name, out_dir);
			if (file_name[strlen(out_dir) - 1] != '/')
			{
				file_name[strlen(out_dir)] = '/';
				for (i = strlen(out_dir) + 1; i < j; i++)
					file_name[i] = input_path[i - strlen(out_dir) - 1];
			}
			else
			{
				for (i = strlen(out_dir); i < j - 1; i++)
					file_name[i] = input_path[i - strlen(out_dir)];
			}
			file_name[j] = '\0';
		}

		// Tworzenie nazw plikow wyjsciowych
		out_file_path = malloc(strlen(file_name + 5));
		strcpy(out_file_path, file_name);
		strcat(out_file_path, ".huf");	
		
		out_key_path = malloc(strlen(file_name + 5));		
		strcpy(out_key_path, file_name);		
		strcat(out_key_path, ".key");	

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
		end_code = malloc(sizeof(char) * (strlen(codes->nodes[0]->code) + 2));
		strcpy(end_code, codes->nodes[0]->code);
		end_code[strlen(codes->nodes[0]->code)] = '0';
		end_code[strlen(codes->nodes[0]->code) + 1] = '\0';

		// Kodowanie do pliku
		in = fopen(input_path, "rb");
		out_file = fopen(out_file_path, "wb");
		out_key = fopen(out_key_path, "wb");

		if (in == NULL || out_file == NULL || out_key == NULL)
		{
			printf("brak uprawnien do tworzenia plikow\n");
			return 3;
		}

		encode(in, out_file, out_key, file_ext, codes, end_code);

		fclose(in);
		fclose(out_file);
		fclose(out_key);

		// Zwalnianie pamięci
		free(file_name);
		free(out_file_path);
		free(out_key_path);
		free(end_code);
		free_word_array(files);
		free_tree(root);
		free_vector(codes);
		free(nodes->nodes);
		free(nodes);
	}

	return 0;
}
