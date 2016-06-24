#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndiff.h"
#include "retrieval.h"

/* ndiff: name-based diff */
void ndiff(int col_a, int col_b, 
	char token_a, char token_b, 
	FILE *ptr_a, FILE *ptr_b, 
	FILE *ptr_aba, FILE *ptr_abb, 
	FILE *ptr_a_b, FILE *ptr_b_a) {

	int isMatch;
	char line_a[LINE_BUFFER];
	char line_b[LINE_BUFFER];
	char pattern_a[PATTERN_SIZE];
	char pattern_b[PATTERN_SIZE];
	Retrieval *root_a = trieGenerate();
	Retrieval *root_b = trieGenerate();
	// build a trie tree according to file_A
	while (fgets(line_a, LINE_BUFFER, ptr_a) != NULL) {
		if (*line_a == '\n') {
			; // skip the empty lines...
		}
		else {
			getPattern(line_a, pattern_a, token_a, col_a);
			trieIncert(pattern_a, &root_a);
		}
	}
	// search and generate target files A&B_B and B-A
	while (fgets(line_b, LINE_BUFFER, ptr_b) != NULL) {
		if (*line_b == '\n') {
			; // skip the empty lines... 
		}
		else {
			getPattern(line_b, pattern_b, token_b, col_b);
			isMatch = trieSearch(pattern_b, root_a);
			if (isMatch) {
				fprintf(ptr_abb, "%s", line_b);
			}
			else {
				fprintf(ptr_b_a, "%s", line_b);
			}
			trieIncert(pattern_b, &root_b);
		}
	}
	free(root_a);
	fseek(ptr_a, 0, SEEK_SET); // move the pointer to the start
	// search and generate target files A&B_A and A-B
	while (fgets(line_a, LINE_BUFFER, ptr_a) != NULL) {
		if (*line_a == '\n') {
			; // skip the empty lines...
		}
		else {
			getPattern(line_a, pattern_a, token_a, col_a);
			isMatch = trieSearch(pattern_a, root_b);
			if (isMatch) {
				fprintf(ptr_aba, "%s", line_a);
			}
			else {
				fprintf(ptr_a_b, "%s", line_a);
			}
		}
	}
	free(root_b);
}