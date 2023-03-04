#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "word_vector.h"
#include "node_vector.h"
#include "huffman.h"
#include "encode.h"


#define BYTE_SIZE 256 	// Rozmiar jednego byte'a


int main(int argc, char **argv)
{
	int i, j;

	// Zmienne getopt
	char opt;
	word_vec_t *files = init_word_vec(2); 	// Wektor przechowuje nazwy plików do skompresowania

	// Czytanie plików
	FILE *in; 				// Plik wejściowy
	int freq[BYTE_SIZE]; 			// Częstość występowania byte'ów w pliku
	int byte; 				// Przechowuje wartości <0; 255>

	// Tworzenie drzewa
	node_vec_t *nodes; 			// Wektor węzłów (bez kodów)
	node_t *root; 				// Korzeń drzewa

	// Kodowanie do pliku
	node_vec_t *codes; 			// Przechowuje węzły z kodami
	char temp_code[BYTE_SIZE]; 		// Tymczasowy ciąg znaków potrzebny do generowania kodów
	char *file_ext; 			// Rozszerzenie originalnego pliku
	FILE *out_file;				// Wyjściowy skompresowany plik
	FILE *out_key;				// Wyjściowy plik z kodami znaków


	// Wczytywanie opcji z getopt
	while ((opt = getopt(argc, argv, "sco:f:")) != -1)
	{
    		switch (opt)
	      	{
	      		case 'f':
				// Dodawanie nowego pliku do komresji
        			add_word(files, optarg);
        			break;
  	    		case '?':
				break;
		}
	}

	// Iterowanie po plikch do skompresowania
	for(j = 0; j < files->n; j++)
	{	
		// Otwieranie pliku
		printf("%s\n", files->words[j]);
		in = fopen(files->words[j], "rb");
		if(in == NULL)
		{
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]);
			return 1;
		}
		
		// Zerowanie częstości byte'ów dla pliku
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
		
		// Dodawanie węzła ze znakiem końca pliku
		add_node(nodes, init_node(-1, 1, NULL, NULL));

		// Tworzenie drzewa
		root = make_tree(nodes);
		printf("root sum: %d\n", root->freq);
	
		// Czytanie kodów z drzewa
		codes = init_node_vec(8);
		temp_code[0] = '\0';
		read_codes(root, codes, temp_code);

		/*
		printf("\n");
		for(i = 0; i < codes->n; i++)
			printf("%d: %s\n", codes->nodes[i]->sign, codes->nodes[i]->code);	
		*/

		// Odczytywanie rozszerzenia oryginalnego pliku
		file_ext = strrchr(files->words[j], '.');
		file_ext++;

		// Kodowanie pliku
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
