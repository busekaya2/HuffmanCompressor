#ifndef WORD_VEC
#define WORD_VEC

/* Struktura dynamicznego wektora przechowującego wskaźniki na ciągi znaków.
 * UWAGA - Wektor nie odpowiada za zwalnianie pamięci zawartości, należy to robić ręcznie*/

typedef struct Word_vec{	
	char **words;
	int n;
	int capacity;
} word_vec_t;

word_vec_t * init_word_vec(int);
int double_word_capacity(word_vec_t*);
int add_word(word_vec_t*, char*);
void free_word_vec(word_vec_t*);

#endif
