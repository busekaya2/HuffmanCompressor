#include <stdio.h>
#include <stdlib.h>

#define MAXBUF 8192


typedef struct Node{
	int value;
	char *code;
	struct Node *left;
	struct Node *right;
} node_t;


int main()
{
	char format;
	FILE *in = fopen("LoremIpsum.key", "r");
	char *buffer[MAXBUF];

	format = getline(buffer, 8192, in);

	printf("format: %s", format);
	return 0;
}
