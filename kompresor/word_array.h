
typedef struct{
	char **words;
	int n;
	int capacity;
} word_array_t;


word_array_t *init_word_array(int size);

int increase_capacity(word_array_t *arr);

int add_word(word_array_t *arr, char* word);

