#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

/* Zamienia jeden bajt na 8 bitów zapisanych jako char* */
char *byte_to_binary(char byte){
	char* binary = (char*) malloc(9 * sizeof(char)); // 8 bitów + '\0'
	if (binary == NULL)
		// Błąd alokacji pamięci
		return NULL;

	for (int i = 7; i >= 0; i--)
		binary[7 - i] = (byte & (1 << i)) ? '1' : '0';

	binary[8] = '\0';

	return binary;
}

/* Funkcja dekompresująca plik ".huf" w celu sprawdzenia poprawności kompresora.
 * Przujmuje wskaźniki na plik wejściowy i wyjściowy oraz korzeń drzewa */
int decode(FILE *in, FILE *out, node_t *root){
	int c, i;			// Zmienne do iterowania
	char* bin;			// Jeden bajt jako 8 bitów
	node_t *head = root;		// Wskaźnik na badany węzeł drzewa
	
	// "Zjadanie" części słownikowej pliku ponieważ mamy już drzewo
	while ((c = fgetc(in)) != '\0') {}

	while ((c = fgetc(in)) != EOF){
		bin = byte_to_binary(c);
		
		if (bin == NULL) 
			// Błąd alokacji pamięci
			return 1;

		for (i = 0; i < strlen(bin); i++){
			// Idziemy w głąb drzewa
			if (bin[i] == '0')
				head = head->left;
			
			if (bin[i] == '1')
				head = head->right;

			if (head->left == NULL && head->right == NULL){
				if (head->sign == -1){
					// Znaleziono znak końca
					free(bin);
					return 0;
				}

				// Zapisywanie bajta do pliku i powrót do korzenia
				fwrite(&(head->sign), 1, sizeof(char), out);
				head = root;
		
			}
		}
		free(bin);
	}

	// Nie znaleziono znaku końca więc kody są nieprawidłowe
	return 2;
}
