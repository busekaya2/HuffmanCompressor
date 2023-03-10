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

void help(){
	printf("Program kompresuje pliki z wykorzystaniem algorytmu Huffmana.\n");
	printf("Przy kompresji tworzony jest plik z roszerzeniem .huf zawierający skompresowane dane oraz klucz do ich dekompresji.\n\n");
	printf("Składnia: ./compressor.exe [-f PLIK] [-s] [-h] [-d]\n\n");
	printf("Opcje:\n");
	printf("\t-f\tŚcieżka pliku, który ma zostać skompresowany\n");
	printf("\t-s\tWyświetlenie rozmiaru pliku przed i po kompresji\n");
	printf("\t-h\tWyświetlenie okna pomocy (help)\n");
	printf("\t-d\tNatychmiast dekompresuje skompresowany plik w celu sprawdzenia poprawności kompresji\n\n");
	printf("Program może kompresować naraz kilka plików. Należy wówczas przed ścieżką każdego pliku wstawić opcję -f.\n");
	printf("\tPrzykład: ./compressor.exe -f file1 -f file2 -f file3\n");
}

// Zwraca rozmiar pliku w KB
long int get_file_size(FILE *f){
	fseek(f, 0L, SEEK_END);
	long int file_size = ftell(f);

	return file_size / 1024;
}

int main(int argc, char **argv){
	int i, j;				// Zmienne do iterowania

	// Zmienne getopt
	char opt;				// Zmienna określająca opcję getopt
	word_vec_t *files = init_word_vec(2); 	// Wektor przechowuje nazwy plików do skompresowania
	int show_file_size = 0;			// Opcja wyświetlenia rozmiaru przed i po komresji
	int decode_file = 0;			// Opcja dekompresji skompresowanego pliku

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
	int file_name_n;			// Długość nazwy oryginalnego pliku bez rozszerzenia
	char *file_name;			// Nazwa oryginalnego pliku bez rozszerzenia
	char *out_file_name;			// Nazwa skompresowanego pliku wyjściowego
	char *out_decoded_name;			// Nazwa zdekompresowanego pliku (opcja wywołania)
	FILE *out_file;				// Wyjściowy skompresowany plik
	FILE *out_decoded;			// Wyjściowy plik po wykonaniu dekompresji skompresowanego pliku (opcja wywyołania)

	// Wczytywanie opcji z getopt
	while ((opt = getopt(argc, argv, "hdsf:")) != -1){
    		switch (opt){
	      		case 'f':
				// Dodawanie nowego pliku do komresji
        			if (add_word(files, optarg) == 1){
					fprintf(stderr, "%s: Błąd alokacji pamięci nazwy pliku: %s\n", argv[0], files->words[j]);
					return 3;
				}
        			break;
	      		case 'd':
				// Opcja dekompresji pliku
				decode_file = 1;
				break;
	      		case 's':
				// Opcja wyświetlenia rozmiaru pliku przed i po kompresji
				show_file_size = 1;
				break;
	      		case 'h':
				// Okno pomocy
				help();
				return 0;
  	    		case '?':
				help();
				return 4;
		}
	}

	if (files->n < 1)
	{
		fprintf(stderr, "%s: Nie podano żadnego pliku.\n\n", argv[0]);
		help();
		return 1;
	}

	// Iterowanie po plikach do skompresowania
	for (j = 0; j < files->n; j++){	
		// Otwieranie pliku
		in = fopen(files->words[j], "rb");
		if (in == NULL){
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]);
			return 1;
		}
		
		// Zerowanie częstości byte'ów dla pliku
		for (i = 0; i < BYTE_SIZE; i++)
			freq[i] = 0;
		
		// Liczenie częstości znaków
		while ((byte = fgetc(in)) != EOF)
			freq[byte]++;
		
		fclose(in);

		// Tworzenie struktur węzłów
		nodes = init_node_vec(8);
		if (nodes == NULL){
			fprintf(stderr, "%s: Błąd alokacji pamięci struktury węzłów: %s\n", argv[0], files->words[j]);
			return 3;
		}

		for (i = 0; i < BYTE_SIZE; i++){
			if(freq[i] != 0){
				if (add_node(nodes, init_node(i, freq[i], NULL, NULL)) == 1){
					fprintf(stderr, "%s: Błąd alokacji pamięci węzła: %s\n", argv[0], files->words[j]);
					return 3;
				}
			}
		}
		
		// Dodawanie węzła ze znakiem końca pliku (-1)
		if (add_node(nodes, init_node(-1, 1, NULL, NULL)) == 1){
			fprintf(stderr, "%s: Błąd alokacji pamięci węzła końcowego: %s\n", argv[0], files->words[j]);
			return 3;
		}

		// Tworzenie drzewa
		root = make_tree(nodes);
		if (root == NULL){
			fprintf(stderr, "%s: Błąd alokacji pamięci drzewa: %s\n", argv[0], files->words[j]);
			return 3;
		}

		// Czytanie kodów z drzewa
		codes = init_node_vec(8);
		if (codes == NULL){
			fprintf(stderr, "%s: Błąd alokacji pamięci struktury węzłów z kodami: %s\n", argv[0], files->words[j]);
			return 3;
		}

		temp_code[0] = '\0';
		read_codes(root, codes, temp_code);

		for (i = 0; i < codes->n; i++){
			if (codes->nodes[i]->code == NULL){
				fprintf(stderr, "%s: Błąd alokacji pamięci kodu: %s\n", argv[0], files->words[j]);
				return 3;
			}
		}

		// Odczytywanie rozszerzenia oryginalnego pliku
		file_ext = strrchr(files->words[j], '.');
		if (file_ext != NULL) {
			file_ext++;
			if ((file_ext - 1) - strrchr(files->words[j], '/') < 0)
				// Sprawdzamy czy znaleziona kropka nalezy do samej nazwy pliku a nie ścieżki
				file_ext = NULL;
		}
			
		// Liczenie długości nazwy originalnego pliku (bez rozszerzenia)
		file_name_n;
		if (file_ext != NULL)
			file_name_n = strlen(files->words[j]) - strlen(file_ext) - 1;
		else
			file_name_n = strlen(files->words[j]);
		
		// Zapisywanie nazwy pliku bez rozszerzenia
		file_name = malloc(sizeof(char) * (file_name_n + 1));
		if (file_name == NULL){
			fprintf(stderr, "%s: Błąd alokacji pamięci nazwy pliku: %s\n", argv[0], files->words[j]);
			return 3;
		}

		for (i = 0; i < file_name_n; i++)
			file_name[i] = files->words[j][i];
		file_name[i] = '\0';
		
		// Tworzenie nazw plików wyjściowych
		out_file_name = malloc(sizeof(char) * (file_name_n + 5));
		if (out_file_name == NULL){
			fprintf(stderr, "%s: Błąd alokacji pamięci nazwy pliku wyjściowego: %s\n", argv[0], files->words[j]);
			return 3;
		}
		
		strcpy(out_file_name, file_name);
		strcat(out_file_name, ".huf\0");
				
		// Kodowanie pliku
		in = fopen(files->words[j], "rb");
		if (in == NULL){
			fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]);
			return 1;
		}

		out_file = fopen(out_file_name, "wb");
		if (out_file == NULL){
			fprintf(stderr, "%s: Brak uprawnień do pliku: %s\n", argv[0], files->words[j]);
			return 2;
		}

		encode(in, out_file, file_ext, codes);
	
		// Wyświetlanie rozmiatu pliku przed i po kompresji (opcja wywołania)
		if (show_file_size){
			printf("%s\n", files->words[j]);
			printf("\tRozmiar przed kompresją: %ld [KB]\n", get_file_size(in));
			printf("\tRozmiar po kompresji: %ld [KB]\n", get_file_size(out_file));
		}

		fclose(in);
		fclose(out_file);

		// Dekodowanie pliku (opcja wywołania)
		if (decode_file){
			if (file_ext != NULL)
				out_decoded_name = malloc(sizeof(char) * (file_name_n + 10 + strlen(file_ext)));
			else
				out_decoded_name = malloc(sizeof(char) * (file_name_n + 9));
			
			if (out_decoded_name == NULL){
				fprintf(stderr, "%s: Błąd alokacji pamięci nazwy pliku wyjściowego: %s\n", argv[0], files->words[j]);
				return 3;
			}

			strcpy(out_decoded_name, file_name);
			strcat(out_decoded_name, "_decoded");
			
			if (file_ext != NULL){
				strcat(out_decoded_name, ".");
				strcat(out_decoded_name, file_ext);
			}
			strcat(out_decoded_name, "\0");
		
			out_decoded = fopen(out_decoded_name, "wb");
			if (out_decoded == NULL){
				fprintf(stderr, "%s: Brak uprawnień do pliku: %s\n", argv[0], files->words[j]);
				return 2;
			}

			out_file = fopen(out_file_name, "rb");
			if (out_file == NULL){
				fprintf(stderr, "%s: Błąd odczytu pliku: %s\n", argv[0], files->words[j]);
				return 1;
			}

			decode(out_file, out_decoded, root);
		
			free(out_decoded_name);

			fclose(out_decoded);
			fclose(out_file);
		}

		// Zwalnianie pamięci by następny plik mógł nadpisać
		free(file_name);
		free(out_file_name);
		free_node_vec(nodes);
		
		for (i = 0; i < codes->n; i++)
			free_node(codes->nodes[i]);
		free_node_vec(codes);
		
		free_tree(root);
	}	
	
	free_word_vec(files);
		
	return 0;
}
