#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retrieval.h"

/* trieGenerate: generate a trienode and initialize it */
Retrieval *trieGenerate(void) {
	int i;
	Retrieval *temp = (Retrieval *) malloc(sizeof(Retrieval));
	temp -> isPattern = 0;
	for (i = 0; i < BRANCH_SIZE; i++) {
		temp -> next[i] = NULL;
	}
	return temp;
}
 
/* trieIncert: incert a pattern string to a trie */
void trieIncert(char *str, Retrieval **pRoot) {
	Retrieval *temp = *pRoot;
	int i;
	while (*str) {
		i = *str;
		if (temp -> next[i]) {
			; // do nothing if already exist
		}
		else {
			temp -> next[i] = trieGenerate();
		}
		str++;
		temp = temp -> next[i];
		if (*str == '\0') {
			temp -> isPattern = 1;
		}
	}
}

/* trieSearch: search for a certain pattern in a trie */
int trieSearch(char *str, Retrieval *root) {
	int i;
	Retrieval *temp = root;
	if (root == NULL){
		return 0;
	}
	while (*str) {
		i = *str;
		if (temp -> next[i]) {
			temp = temp -> next[i];
		}
		else {
			return 0;
		}
		str++;
	}
	if (temp -> isPattern == 1) {
		return 1;
	}
	else {
		return 0;
	}
}