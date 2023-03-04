#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "word_vector.h"
#include "node_vector.h"
#include "huffman.h"
#include "encode.h"

#define BYTE_SIZE 256

int main(int argc, char **argv)
{
	int i, j;

	// Zmienne getopt
	char opt; 
	word_vec_t *files = init_word_vec(2);

	// Czytanie plików
	FILE *in;
	int freq[BYTE_SIZE];
	int byte;

	// Tworzenie drzewa
	node_vec_t *nodes;
	node_t *root;

	// Generowanie znaku końca
	int min;

	// Kodowanie do pliku
	node_vec_t *codes;
	char temp_code[BYTE_SIZE];
	char *file_ext;
	FILE *out_file;
	FILE *out_key;

	while ((opt = getopt(argc, argv, "sco:f:")) != -1)
	{
    		switch (opt)
	      	{
	      		case 'f':
        			add_word(files, optarg);
        			break;
  	    		case '?':
				break;
		}
	}

	for(j = 0; j < files->n; j++)
	{	
		printf("%s\n", files->words[j]);
		in = fopen(files->words[j], "rb");
		if(in == NULL)
		{
			fprintf(stderr, "%s: błąd odczytu pliku %s\n", argv[0], files->words[j]);
			return 1;
		}
		
		// Zerowanie częstości byte'ów dla kolejnego pliku
		for(i = 0; i < BYTE_SIZE; i++)
			freq[i] = 0;
		
		// Liczenie częstości znaków
		while((byte = fgetc(in)) != EOF)
			freq[byte]++;
		
		fclose(in);

		// Tworzenie struktur węzłów
		nodes = init_node_vec(8);
		for(i = 0; i < BYTE_SIZE; i++)
			if(freq[i] != 0)
				add_node(nodes, init_node(i, freq[i], NULL, NULL));

		// Tworzenie drzewa
		root = make_tree(nodes);
		printf("root sum: %d\n", root->freq);
	
		// Czytanie kodów z drzewa
		codes = init_node_vec(8);
		temp_code[0] = '\0';
		read_codes(root, codes, temp_code);

		// Generowanie znaku końca (przesunięcie wybranego znaku o poziom w głąb drzewa i dodanie obok znaku końca)
		 	// Szukanie najdluzszego kodu
		min = 0;
		for (i = 1; i < codes->n; i++)
			if (codes->nodes[i]->freq < codes->nodes[min]->freq)
				min = i;
			
			// Tworzenie dwóch nowych liści
		add_node(codes, init_node(codes->nodes[min]->sign, codes->nodes[min]->freq, NULL, NULL));
		add_node(codes, init_node(-1, 0, NULL, NULL));

			// Łączenie liści z drzewem
		codes->nodes[min]->left = codes->nodes[codes->n - 1];
		codes->nodes[min]->right = codes->nodes[codes->n - 2];
			
			// Generowanie kodow
		codes->nodes[codes->n - 1]->code = malloc(sizeof(char) * (strlen(codes->nodes[min]->code) + 2));
		codes->nodes[codes->n - 2]->code = malloc(sizeof(char) * (strlen(codes->nodes[min]->code) + 2));
		strcpy(codes->nodes[codes->n - 1]->code, codes->nodes[min]->code);
		strcpy(codes->nodes[codes->n - 2]->code, codes->nodes[min]->code);
		strcat(codes->nodes[codes->n - 1]->code, "0\0");
		strcat(codes->nodes[codes->n - 2]->code, "1\0");

			// Zwalnianie 
		//free(codes->nodes[min]->code);
		free_node(codes->nodes[min]);
		remove_node(codes, min);
		
		printf("\n");
		for(i = 0; i < codes->n; i++)
			printf("%d: %s\n", codes->nodes[i]->sign, codes->nodes[i]->code);	

		// Odczytywanie rozszerzenia pliku
		file_ext = strrchr(files->words[j], '.');
		file_ext++;
		//printf("%s\n", file_ext);

		in = fopen(files->words[j], "rb");
		out_file = fopen("test.huf", "wb");
		out_key = fopen("test.key", "w");
		encode(in, out_file, out_key, file_ext, codes);
		fclose(in);
		fclose(out_file);
		fclose(out_key);

		// Zwalnianie pamięci dla następnego pliku
		free_node_vec(nodes);
		for (i = 0; i < codes->n; i++)
			free_node(codes->nodes[i]);
		free_node_vec(codes);
		free_tree(root);
	}	
	
	free_word_vec(files);
		
	return 0;
}
