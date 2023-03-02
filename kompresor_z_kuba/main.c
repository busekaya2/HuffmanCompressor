#include <stdio.h>
#include <unistd.h>
#include "word_vector.h"
#include "node_vector.h"
#include "huffman.h"

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

	// Kodowanie do pliku
	node_vec_t *codes;

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
			fprintf(stderr, "%s: błąd odczytu pliku %s", argv[0], files->words[j]);
			return 1;
		}
		
		// Zerowanie głównych zmiennych dla kolejnego pliku
		for(i = 0; i < BYTE_SIZE; i++)
			freq[i] = 0;
		
		// Liczenie częstości znaków
		while((byte = fgetc(in)) != EOF)
			freq[byte]++;
		
		// Dodanie znaku końca pliku
		freq[26]++;

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
		

		// Zwalnianie pamięci dla następnego pliku
		free_node_vec(nodes);
		free_node_vec(codes);
	}	
	
	free_word_vec(files);
		
	return 0;
}
