#include <stdlib.h>
#include "word_array.h"


word_array_t *init_word_array(int size)
{
	if (size <= 0)
		return NULL;

	word_array_t *new = malloc(sizeof(word_array_t));
	
	if (new == NULL)
		return NULL;
	
	new->words = malloc(sizeof(char*) * size);
	
	if (new->words == NULL)
		return NULL;
	
	new->capacity = size;
	new->n = 0;
	
	return new;
}

int increase_capacity(word_array_t *arr)
{
	arr->capacity *= 2;
	if (realloc(arr->words, sizeof(char*) * arr->capacity) != NULL)
		return 0;
	else
		return 1;
}

int add_word(word_array_t *arr, char* word)
{
	if (arr->n >= arr->capacity)
		if (increase_capacity(arr) != 0)
			return 1;
	
	arr->words[arr->n++] = word;
	
	return 0;
}			


