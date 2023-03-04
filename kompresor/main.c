#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "word_vector.h"
#include "node_vector.h"
#include "huffman.h"
#include "encode.h"
#include "decode.h"


#define BYTE_SIZE 256 	// Rozmiar jednego bajta


int main(int argc, char **argv)
{
	int i, j;				// Zmienne do iterowania

	// Zmienne getopt
	char opt;				// Zmienna określająca opcję getopt
	word_vec_t *files = init_word_vec(2); 	// Wektor przechowuje nazwy plików do skompresowania

	// Czytanie plików do kompresji
	FILE *in; 				// Plik wejściowy
	int freq[BYTE_SIZE]; 			// Częstość występowania bajtów w pliku
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
	FILE *out_decoded;			// Wyjściwy plik po wykonaniu dekompresji wcześniej skompresowanego pliku (opcja wywyołania)


	// Wczytywanie opcji z getopt
	while ((opt = getopt(argc, argv, "dsco:f:")) != -1)
	{
    		switch (opt)
	      	{
	      		case 'f':
				// Dodawanie nowego pliku do komresji
        			if (add_word(files, optarg) == 1) {
					fprintf(stderr, "%s: Błąd alokacji pamięci nazwy pliku: %s\n", argv[0], files->words[j]); return 3;
				}
        			break;
  	    		case '?':
				break;
		}
	}

	// Iterowanie po plikch do skompresowania
	for(j = 0; j < files->n; j++)
	{	
		// Otwieranie pliku
		in = fopen(files->words[j], "rb");
		if(in == NULL) {
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]); return 1;
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

		if (nodes == NULL) {
			fprintf(stderr, "%s: Błąd alokacji pamięci struktury węzłów: %s\n", argv[0], files->words[j]); return 3;
		}

		for(i = 0; i < BYTE_SIZE; i++)
		{
			if(freq[i] != 0)
			{
				if (add_node(nodes, init_node(i, freq[i], NULL, NULL)) == 1) {
					fprintf(stderr, "%s: Błąd alokacji pamięci węzła: %s\n", argv[0], files->words[j]); return 3;
				}
			}
		}
		
		// Dodawanie węzła ze znakiem końca pliku (-1)
		if (add_node(nodes, init_node(-1, 1, NULL, NULL)) == 1) {
			fprintf(stderr, "%s: Błąd alokacji pamięci węzła końcowego: %s\n", argv[0], files->words[j]); return 3;
		}

		// Tworzenie drzewa
		root = make_tree(nodes);
		if (root == NULL) {
			fprintf(stderr, "%s: Błąd alokacji pamięci drzewa: %s\n", argv[0], files->words[j]); return 3;
		}

		// Czytanie kodów z drzewa
		codes = init_node_vec(8);
		if (codes == NULL) {
			fprintf(stderr, "%s: Błąd alokacji pamięci struktury węzłów z kodami: %s\n", argv[0], files->words[j]); return 3;
		}


		temp_code[0] = '\0';
		read_codes(root, codes, temp_code);

		for (i = 0; i < codes->n; i++) {
			if (codes->nodes[i]->code == NULL) {
				fprintf(stderr, "%s: Błąd alokacji pamięci kodu: %s\n", argv[0], files->words[j]); return 3;
			}
		}

		// Odczytywanie rozszerzenia oryginalnego pliku
		file_ext = strrchr(files->words[j], '.');
		if (file_ext != NULL)
			file_ext++;

		// Kodowanie pliku
		in = fopen(files->words[j], "rb");
		if (in == NULL) {
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]); return 1;
		}

		out_file = fopen("test.huf", "wb");
		if (out_file == NULL) {
			fprintf(stderr, "%s: Brak uprawnień do pliku: %s\n", argv[0], files->words[j]); return 2;
		}

		out_key = fopen("test.key", "w");
		if (out_key == NULL) {
			fprintf(stderr, "%s: Brak uprawnień do pliku: %s\n", argv[0], files->words[j]); return 2;
		}

		encode(in, out_file, out_key, file_ext, codes);

		fclose(in);
		fclose(out_file);
		fclose(out_key);

		// Dekodowanie pliku (opcja wywołania)
		out_decoded = fopen("decoded.txt", "wb");
		if (out_decoded == NULL) {
			fprintf(stderr, "%s: Brak uprawnień do pliku: %s\n", argv[0], files->words[j]); return 2;
		}

		out_file = fopen("test.huf", "rb");
		if (out_file == NULL) {
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]); return 1;
		}

		decode(out_file, out_decoded, codes);
		
		fclose(out_decoded);
		fclose(out_file);

		// Zwalnianie pamięci by następny plik mógł nadpisać
		free_node_vec(nodes);
		for (i = 0; i < codes->n; i++)
			free_node(codes->nodes[i]);
		free_node_vec(codes);
		free_tree(root);
	}	
	
	free_word_vec(files);
		
	return 0;
}
