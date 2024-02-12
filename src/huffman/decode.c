#include "decode.h"
#include "../node/node.h"
#include "../utils/bit_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int decode(FILE *in, FILE *out, node_t *root) {
	int i, c;
	char* bin;		
	node_t *head = root;	
	
	// "Eat" dictionary
	while ((c = fgetc(in)) != '\0') {}

	while ((c = fgetc(in)) != EOF) {
		bin = byte_to_binary(c);
		
		if (bin == NULL) {
			// Błąd alokacji pamięci
			return 1;
		}

		for (i = 0; i < strlen(bin); i++) {
			// Idziemy w głąb drzewa
			if (bin[i] == '0') {
				head = head->left;
			}
			
			if (bin[i] == '1') {
				head = head->right;
			}

			if (head->left == NULL && head->right == NULL) {
				if (head->sign == -1) {
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
