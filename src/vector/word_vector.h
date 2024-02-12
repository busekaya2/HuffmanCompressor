#ifndef WORD_VEC_H
#define WORD_VEC_H


/* Structure of a dynamic vector holding pointers to strings.
 * NOTE - The vector is not responsible for freeing the memory of the contents, it must be done manually */

typedef struct word_vec{	
	char **words;
	int n;
	int capacity;
} word_vec_t;

word_vec_t * init_word_vec(int);
int add_word(word_vec_t*, char*);
void free_word_vec(word_vec_t*);


#endif
