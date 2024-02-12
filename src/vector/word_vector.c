#include "word_vector.h"
#include <stdlib.h>


/* Structure of a dynamic vector holding pointers to strings.
 * NOTE - The vector is not responsible for freeing the memory of the contents, it must be done manually */

word_vec_t * init_word_vec(int capacity){
	word_vec_t *new = malloc(sizeof(word_vec_t));
	
	if (new == NULL)
		// Memory alloc error
		return NULL;
	
	new->words = malloc(sizeof(char*) * capacity);
	
	if (new->words == NULL)
		// Memory alloc error
		return NULL;

	new->capacity = capacity;
	new->n = 0;
	
	return new;
}

int double_word_capacity(word_vec_t* vec){
	vec->capacity *= 2;
	vec->words = realloc(vec->words, sizeof(char*) * vec->capacity);
	
	if (vec->words == NULL)
		// Memory alloc error
		return 1;

	return 0;
}

int add_word(word_vec_t *vec, char *word){
	if (vec->n >= vec->capacity)
		if (double_word_capacity(vec) == 1)
			// Memory alloc error
			return 1;
	
	vec->words[vec->n++] = word;

	return 0;
}

void free_word_vec(word_vec_t *vec){
	free(vec->words);
	free(vec);
}
