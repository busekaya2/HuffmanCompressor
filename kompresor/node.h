#ifndef NODE_H
#define NODE_H

typedef struct node{
	int value;
	int frequency;
	struct node *left_child;
	struct node *right_child;
	char* code;
} node_t;

node_t* init_node(int, int, node_t*, node_t*);

#endif
