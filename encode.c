#include <stdio.h>
#include <string.h>
#include "node_vector.h"

/* Szuka kodów w wektorze i zwraca ten pasujący do danego znaku */
char *find_code(int sign, node_vec_t *codes){
	int i;

	for (i = 0; i < codes->n; i++)
		if(codes->nodes[i]->sign == sign)
			return codes->nodes[i]->code;
	
	return NULL;
}

/* Funkcja kodująca znaki do pliku oraz tworząca plik klucz
 * Czytamy kolejne bajty pliku, znajdujemy odpowiednie kody a następnie ładujemy je do pliku wyjściowego.
 * Na sam koniec ładujemy znak końca pliku (-1) i uzupełniamy zerami do pełnego bajta jeśli trzeba. */
void encode(FILE *in, FILE *out_file, char* file_ext, node_vec_t *codes){
	char byte = 0;			// Wartość bajta który będzie zapisywany do pliku wyjściowego.
	int shift = 0;			// Przesunięcie bitowe zmiennej byte.
	char* code;			// Wskaźnik na kod znaku
	int i, c;			// Zmienne do iterowania

	/* Zapisywanie rozszerzenia oryginalnego pliku jako pierwsza linia.
	 * Pierwsza linia pusta jeśli brak rozszerzenia (sam znak '\n')*/
	if (file_ext != NULL)
		fprintf(out_file, "%s\n", file_ext);
	else
		fprintf(out_file, "\n");
	
	// Zapisywanie znaków i kodów do pliku.
	for (i = 0; i < codes->n; i++)
		fprintf(out_file, "%d %s\n", codes->nodes[i]->sign, codes->nodes[i]->code);
	
	// Znak '\0' mówiący o końcu części słownikowej
	fwrite(&byte, 1, sizeof(byte), out_file);

	// Ładowanie kodów znaków do pliku
	while ((c = getc(in)) != EOF){
		// Szukamy kodu znaku
		code = find_code(c, codes);

		// Zapisujemy bit po bicie do pliku wyjściowego
		for (i = 0; i < strlen(code); i++){
			if (shift > 7){
				/* Jeśli przesunięto już o 8 bitów to zapisujemy do pliku.
				 * Zerujemy bajt */
				fwrite(&byte, 1, sizeof(byte), out_file);
				byte = 0;
				shift = 0;
			}

			// Zapisywanie bitu do bajta
			byte |= ((code[i] - '0') << (7 - shift));
			shift++;
		}
	}

	//  Ładowanie znaku końca (-1) do pliku
	code = find_code(-1, codes);		
	for (i = 0; i < strlen(code); i++){
		if (shift > 7){
			fwrite(&byte, 1, sizeof(byte), out_file);
			byte = 0;
			shift = 0;
		}
		byte |= ((code[i] - '0') << (7 - shift));
		shift++;
	}
		
	// Uzupełnianie zerami jeśli nie wypełniono całego bajta
	while (shift < 8){
		byte |= (0 << (7 - shift));
		shift++;
	}

	fwrite(&byte, 1, sizeof(byte), out_file);
}
