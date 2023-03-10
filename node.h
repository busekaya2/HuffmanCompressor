#ifndef NODE
#define NODE

/* Struktura odpowiada za przechowywanie węzła.
 * Posiada informacje o znaku, częstości, kodzie, prawym i lewym wskażniku na kolejny węzeł. */

typedef struct Node{
	int sign;
	int freq;
	char *code;
	struct Node *left;
	struct Node *right;
} node_t;

node_t * init_node(int, int, node_t *, node_t *);
void free_node(node_t *);

#endif
