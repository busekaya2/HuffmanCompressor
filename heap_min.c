#include <stdlib.h>
#include <stdio.h>
#include "heap_min.h"
#include "node.h"

heap_min_t *init_heap_min(int initial_size) {
    heap_min_t *heap;
    
    if ((heap = malloc(sizeof(heap_min_t))) == NULL) {
        // Heap memory alloc error
        return NULL;
    }
    if ((heap->elements = malloc(sizeof(node_t*) * initial_size)) == NULL) {
        // Heap node array memory alloc error
        free(heap);
        return NULL;
    }

    heap->capacity = initial_size;
    heap->n = 0;

    return heap;
}

int heap_min_double_size(heap_min_t *heap) {
    heap->capacity *= 2;
    
    if ((heap->elements = realloc(heap->elements, sizeof(node_t*) * heap->capacity)) == NULL) {
        // Memory alloc error
        return 1;
    }

    return 0;
}

int heap_min_insert(heap_min_t *heap, node_t *element) {
    int result;
    int head_id;
    int parent_id;

    if (element == NULL) {
        // Invalid input node
        return 2;
    }

    if (heap->n + 1 >= heap->capacity) {
        if (heap_min_double_size(heap) != 0) {
            // Memory alloc error
            return 1;
        }
    }

    head_id = heap->n++;
    heap->elements[head_id] = element;
    parent_id = (head_id - 1) / 2;
    
    // Move node up until heap is fixed
    while (heap->elements[head_id]->freq < heap->elements[parent_id]->freq) {
        heap_min_swap(heap, head_id, parent_id);
        head_id = parent_id;
        parent_id = (head_id - 1) / 2;
    }

    return 0;
}

node_t *heap_min_remove(heap_min_t *heap) {
    node_t *to_return;

    if (heap->n < 1) {
        return NULL;
    }

    // Remove root node and fix heap
    to_return = heap->elements[0];
    heap->elements[0] = heap->elements[--heap->n];
    heap_min_heapify(heap, 0);
    
    return to_return;
}

void heap_min_heapify(heap_min_t *heap, int head_id) {
    int left_child_id = (head_id * 2) + 1;
    int right_child_id = (head_id * 2) + 2;
    int min_child_id = head_id;

    if (left_child_id < heap->n && heap->elements[left_child_id]->freq < heap->elements[min_child_id]->freq) {
        min_child_id = left_child_id;
    }
    if (right_child_id < heap->n && heap->elements[right_child_id]->freq < heap->elements[min_child_id]->freq) {
        min_child_id = right_child_id;
    }

    // Swap with smaller child until heap is fixed 
    if (min_child_id != head_id) {
        heap_min_swap(heap, head_id, min_child_id);
        heap_min_heapify(heap, min_child_id);
    }
}

// Function won't free it's content, you have to do it manually
void heap_min_free(heap_min_t* heap) {
    free(heap->elements);
    free(heap);
}

void heap_min_swap(heap_min_t *heap, int id_a, int id_b) {
    node_t *temp = heap->elements[id_a];
    heap->elements[id_a] = heap->elements[id_b];
    heap->elements[id_b] = temp;
}
