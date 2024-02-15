#ifndef NODE_H
#define NODE_H


typedef struct node {
	unsigned char sign;
	unsigned int freq;
	struct node *left;
	struct node *right;
} node_t;

node_t *init_node(unsigned char, unsigned int, node_t*, node_t*);
void free_node(node_t*);


#endif
