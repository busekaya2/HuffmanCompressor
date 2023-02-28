#include <stdio.h>
#include <string.h>
#include "vector.h"


char *find_code(char sign, vector_t *codes)
{
	int i;

	for (i = 0; i < codes->n; i++)
	{
		if (sign == (codes->nodes[i])->value)
			return (codes->nodes[i])->code;
	}

	return NULL;
}


void encode(FILE *in, FILE *out_file, FILE *out_key, char* file_ext, vector_t *codes, char *end_code)
{
	char byte = 0;
	int c, i;
	int shift = 0;
	char* code;

	while (c = getc(in))
	{
		if (c != EOF)
			code = find_code(c, codes);
		else
			code = end_code;

		for (i = 0; i < strlen(code); i++)
		{
			if (shift > 7)
			{
				fwrite(&byte, 1, sizeof(byte), out_file);
				byte = 0;
				shift = 0;
			}

			byte |= ((code[i] - '0') << (7 - shift));
			shift++;
		}

		if (c == EOF)
			break;
	}
	while (shift < 8)
	{
		byte |= (0 << (7 - shift));
		shift++;
	}
	fwrite(&byte, 1, sizeof(byte), out_file);

	if (file_ext != NULL)
		fprintf(out_key, "%s\n", file_ext);
	else
		fprintf(out_key, "\n");
	fprintf(out_key, "%s\n", end_code);
	for (i = 0; i < codes->n; i++)
		fprintf(out_key, "%d %s\n", codes->nodes[i]->value, codes->nodes[i]->code);
}
