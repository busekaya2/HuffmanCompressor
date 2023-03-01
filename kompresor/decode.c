#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "vector.h"


char* byte_to_binary(unsigned char byte) {
    char* binary = (char*) malloc(9 * sizeof(char)); // 8 bitów + null terminator
    if (binary == NULL) {
        fprintf(stderr, "Nie udało się przydzielić pamięci");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    binary[8] = '\0';
    
    return binary;
}

void decode(FILE *in, FILE *out, vector_t *codes, char *end_code)
{
	int c, i, j;
	char str[999];
	int str_i = 0;
	char* bin;

	while ((c = fgetc(in)) != EOF)
	{
		bin = byte_to_binary(c);
		
		for (i = 0; i < strlen(bin); i++)
		{
			str[str_i++] = bin[i];
			str[str_i] = '\0';

			for (j=0;j<codes->n;j++)
			{
				if (strcmp(str, codes->nodes[j]->code) == 0)
				{
					fwrite(&(codes->nodes[j]->value), 1, sizeof(char), out);
					str_i = 0;
					str[0] = '\0';
					break;
				}
			}
			if (strcmp(str, end_code) == 0)
				break;
		}

		free(bin);
	}
}
