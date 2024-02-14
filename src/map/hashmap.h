#ifndef HASHMAP_H
#define HASHMAP_H


typedef struct list_node {
    int key;
    char *value;
    struct list_node *next;
} list_node_t;

list_node_t *init_list_node(int, char*, list_node_t*);
void free_list(list_node_t*);

// Struct takes ownership of the value memory.
typedef struct {
    int n;
    list_node_t **buckets;
} hashmap_t;

hashmap_t *init_hashmap(int);
unsigned int hash_func(hashmap_t*, int);
int hashmap_insert(hashmap_t*, int, char*);
char *hashmap_get(hashmap_t*, int);
void free_hashmap(hashmap_t*);


#endif