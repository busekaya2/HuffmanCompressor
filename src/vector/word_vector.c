#include "word_vector.h"
#include "../error_codes.h"
#include <stdlib.h>


/* Structure of a dynamic vector holding pointers to strings.
 * NOTE - The vector is not responsible for freeing the memory of the contents, it must be done manually */

word_vec_t *init_word_vec(int capacity) {
	word_vec_t *new;
	
	new = malloc(sizeof(word_vec_t));
	if (new == NULL) {
		return NULL;
	}
	
	new->words = malloc(sizeof(char*) * capacity);
	if (new->words == NULL) {
		return NULL;
	}

	new->capacity = capacity;
	new->n = 0;
	
	return new;
}

int double_word_capacity(word_vec_t* vec){
	vec->capacity *= 2;
	vec->words = realloc(vec->words, sizeof(char*) * vec->capacity);
	
	if (vec->words == NULL) {
		return ERROR_MEMORY_ALLOC;
	}

	return EXIT_SUCCESS;
}

int add_word(word_vec_t *vec, char *word) {
	if (vec->n >= vec->capacity) {
		if (double_word_capacity(vec) == ERROR_MEMORY_ALLOC) {
			return ERROR_MEMORY_ALLOC;
		}
	}
	
	vec->words[vec->n++] = word;

	return EXIT_SUCCESS;
}

void free_word_vec(word_vec_t *vec){
	if (vec != NULL) {
		free(vec->words);
		free(vec);
	}
}
