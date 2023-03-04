#include "word_vector.h"
#include <stdlib.h>

word_vec_t * init_word_vec(int capacity)
{
	word_vec_t *newvec = malloc(sizeof(word_vec_t));
	if(newvec == NULL)
		return NULL;
	
	newvec->words = malloc(sizeof(char*) * capacity);
	if(newvec->words == NULL)
		return NULL;

	newvec->capacity = capacity;
	newvec->n = 0;
	
	return newvec;
}

int double_word_capacity(word_vec_t* vec)
{
	vec->capacity *= 2;
	vec->words = realloc(vec->words, sizeof(char*) * vec->capacity);
	if(vec == NULL)
		return 1;

	return 0;
}

int add_word(word_vec_t *vec, char *word)
{
	if(vec->n >= vec->capacity)
		if(double_word_capacity(vec) == 1)
			return 1;
	
	vec->words[vec->n++] = word;

	return 0;
}

/* UWAGA - Funkcja nie zwalnia zawartości wektora, jedynie strukturę.
 * Zawartość należy zwolnić ręcznie */
void free_word_vec(word_vec_t *vec)
{
	free(vec->words);
	free(vec);
}
