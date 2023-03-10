#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node_vector.h"
#include "node.h"

// Funkcja porównuje dwa węzły pod względem częstości występowania znaku.
int node_cmp(const void *a, const void *b){
	return (*(node_t**)a)->freq - (*(node_t**)b)->freq;
}

// Funkcja tworzy strukturę drzewa Huffmana i na koniec zwraca węzeł będący korzeniem tego drzewa.
node_t * make_tree(node_vec_t *nodes){
	int i;
	node_t *temp;

	qsort(nodes->nodes, nodes->n, sizeof(node_t*), node_cmp);
	
	while (nodes->n > 1){
		// Łączenie dwóch pierweszych węzłów ze sobą
		nodes->nodes[0] = init_node(0, nodes->nodes[0]->freq + nodes->nodes[1]->freq, nodes->nodes[0], nodes->nodes[1]);

		if (nodes->nodes[0] == NULL)
			// Błąd alokacji pamięci
			return NULL;

		// Przesywanie reszty węzłów by wypełnić lukę
		for (i = 1; i < nodes->n - 1; i++)
            		nodes->nodes[i] = nodes->nodes[i + 1];
		
		nodes->n--;
		
		// Przesuwanie stworzonego węzła by wektor był dalej posortowany
		for (i = 0; i < nodes->n - 1 && (nodes->nodes[i])->freq > (nodes->nodes[i + 1])->freq; i++){
            		temp = nodes->nodes[i];
			nodes->nodes[i] = nodes->nodes[i + 1];
			nodes->nodes[i + 1] = temp;
		}
	}

	return nodes->nodes[0];
}


// Funckja porusza się po dzrewie Huffmana i zapisuje znaki i ich kody do wektora węzłów
void read_codes(node_t *head, node_vec_t *codes, char *temp_code){
	char *tmp;

	if (head->left != NULL){
		strcat(temp_code, "0\0");
		read_codes(head->left, codes, temp_code);
	}

	if (head->right != NULL){
		strcat(temp_code, "1\0");
		read_codes(head->right, codes, temp_code);
	}

	if (head->left == NULL && head->right == NULL){
		/* Alokowanie pamięci dla ciągu znaków przechowującego kod
		 * Pamięc zostanie zwolniona przy wywołaniu funkcji zwalniającej cały węzęł. */
		tmp = malloc(sizeof(char) * (strlen(temp_code) + 1));
		strcpy(tmp, temp_code);
		add_node(codes, init_node(head->sign, head->freq, NULL, NULL));
		codes->nodes[codes->n - 1]->code = tmp;
	}	

	if (strlen(temp_code) > 0)
		temp_code[strlen(temp_code) - 1] = '\0';
}


// Zwalnia węzły należące do drzewa Huffmana
void free_tree(node_t *root){	
	if (root->left != NULL)
		free_tree(root->left);		

	if (root->right != NULL)
		free_tree(root->right);

	free_node(root);
}
