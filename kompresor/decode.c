#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_vector.h"


/* Zamienia jeden byte na 8 bitów */
char *byte_to_binary(char byte)
{
	char* binary = (char*) malloc(9 * sizeof(char)); // 8 bitów + '\0'
	if (binary == NULL)
	{
		// Błąd alokacji pamięci
		return NULL;
	}

	for (int i = 7; i >= 0; i--)
	{
		binary[7 - i] = (byte & (1 << i)) ? '1' : '0';
	}
	binary[8] = '\0';

	return binary;
}


/* Funkcja dekompresująca plik ".huf" w celu sprawdzenia poprawności kompresora.
 * Przujmuje wskaźniki na plik wejściowy i wyjściowy oraz wektor przechowujący kody znaków */
int decode(FILE *in, FILE *out, node_vec_t *codes)
{
	int c, i, j;		// Zmienne do iterowania
	char str[999]; 		// String bitów z pliku
	int str_i = 0;		// Ile bitów jest w stringu
	char* bin;		// Jeden bajt jako 8 bitów
	
	while ((c = fgetc(in)) != EOF)
	{
		bin = byte_to_binary(c);
		
		// Błąd alokacji pamięci
		if (bin == NULL) 
			return 1;

		// Dodajemy bit do stringa
		for (i = 0; i < strlen(bin); i++)
		{
			str[str_i++] = bin[i];
			str[str_i] = '\0';

			// Szukamy kodu w wektorze
			for (j = 0; j<codes->n; j++)
			{
				if (strcmp(str, codes->nodes[j]->code) == 0)
				{
					// Znak końca pliku, jak znajdziemy to nara
					if (codes->nodes[j]->sign == -1)
					{	
						free(bin);
						return 0;
					}
					
					// Zapisz bajt to pliku
					fwrite(&(codes->nodes[j]->sign), 1, sizeof(char), out);
					
					// Wyczyść string bitów
					str_i = 0;
					str[0] = '\0';
					break;
				}
			}
		}
		free(bin);
	}

	// Nie znaleziono znaku końca więc kody są nieprawidłowe
	return 2;
}
