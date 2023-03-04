#ifndef NODE
#define NODE


/* Struktura odpowiada za przchowywanie węzła.
 * Posiada informacje o znaku, częstości, kodzie, prawym i lewym wskażniku na kolejny węzęł. */


typedef struct Node
{
	int sign;
	int freq;
	char *code;
	struct Node *left;
	struct Node *right;
} node_t;

node_t * init_node(int, int, node_t *, node_t *);
void free_node(node_t *);

#endif
