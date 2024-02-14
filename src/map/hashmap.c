#include "hashmap.h"
#include "../error_codes.h"
#include <stdlib.h>


list_node_t *init_list_node(int key, char *value, list_node_t *next) {
    list_node_t *node;
    
    node = malloc(sizeof(list_node_t));
    if (node == NULL) {
        return NULL;
    }

    node->key = key;
    node->value = value;
    node->next = next;

    return node;
}

void free_list(list_node_t *head) {
    if (head == NULL) {
        return;
    }

    if (head->next != NULL) {
        free_list(head->next);
    }

    if (head->value != NULL) {
        free(head->value);
    }
    free(head);
}

hashmap_t *init_hashmap(int size) {
    int i;
    hashmap_t *map;
    
    map = malloc(sizeof(hashmap_t));
    if (map == NULL) {
        return NULL;
    }

    map->n = size;
    map->buckets = malloc(sizeof(list_node_t*) * size);
    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }

    for (i = 0; i < size; i++) {
        map->buckets[i] = NULL;
    }

    return map;
}

unsigned int hash_func(hashmap_t *map, int key) {
    return key % map->n;
}

// Function takes ownership of the value memory.
int hashmap_insert(hashmap_t *map, int key, char *value) {
    int index;
    list_node_t *node, *head, *prev = NULL;

    index = hash_func(map, key);
    head = map->buckets[index];

    while (head != NULL) {
        if (head->key == key) {
            free(head->value);
            head->value = value;
            return EXIT_SUCCESS;
        }
        prev = head;
        head = head->next;
    }

    node = init_list_node(key, value, NULL);
    if (node == NULL) {
        return ERROR_MEMORY_ALLOC; 
    }

    if (prev == NULL) {
        map->buckets[index] = node;
    } else {
        prev->next = node;
    }

    return EXIT_SUCCESS;
}

char *hashmap_get(hashmap_t *map, int key) {
    list_node_t *head;
    int index;

    head = map->buckets[hash_func(map, key)];
    while (head != NULL) {
        if (head->key == key) {
            return head->value;
        }

        head = head->next;
    }

    return NULL;
}

void free_hashmap(hashmap_t *map) {
    int i;

    if (map == NULL) {
        return;
    } 

    for (i = 0; i < map->n; i++) {
        if (map->buckets[i] != NULL) {
            free_list(map->buckets[i]);
        }
    }

    free(map->buckets);
    free(map);
}