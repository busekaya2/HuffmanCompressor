#include <stdio.h>
#include <unistd.h>
#include "word_vector.h"

int main(int argc, char **argv)
{
	int i;

	// Zmienne getopt
	char opt; 
	word_vec_t *files = init_word_vec(2);
	
	while ((opt = getopt(argc, argv, "sco:f:")) != -1)
	{
    		switch (opt)
	      	{
	      		case 'f':
        			add_word(files, optarg);
        			break;
  	    		case '?':
				break;
		}
	}

	for(i = 0; i < files->n; i++)
		printf("%s\n", files->words[i]);

	free_word_vec(files);
		
	return 0;
}
