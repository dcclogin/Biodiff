/*
* @Author: rumia
* @Date:   2016-08-08 23:23:45
* @Last Modified by:   rumia
* @Last Modified time: 2016-08-11 03:42:19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdiff.h"
#include "retrieval.h"

/* cdiff: coordinate-based diff */
void cdiff(char *cols_a, char *cols_b,
	FILE *ptr_a, FILE *ptr_b,
	FILE *ptr_aba, FILE *ptr_abb,
	FILE *ptr_a_b, FILE *ptr_b_a) {

	char col_a1[5], col_a2[5];
	char col_b1[5], col_b2[5];
	getPattern(cols_a, col_a1, ',', 1);
	getPattern(cols_a, col_a2, ',', 2);
	getPattern(cols_b, col_b1, ',', 1);
	getPattern(cols_b, col_b2, ',', 2);
	int icol_a1 = atoi(col_a1);
	int icol_a2 = atoi(col_a2);
	int icol_b1 = atoi(col_b1);
	int icol_b2 = atoi(col_b2);

	int isMatch;
	char line_a[LINE_BUFFER];
	char line_b[LINE_BUFFER];
	char pattern_a1[PATTERN_SIZE];
	char pattern_a2[PATTERN_SIZE];
	char pattern_b1[PATTERN_SIZE];
	char pattern_b2[PATTERN_SIZE];
	Retrieval *root_a = trieGenerate();
	Retrieval *root_b = trieGenerate();
	// build a trie tree according to file_A
	while (fgets(line_a, LINE_BUFFER, ptr_a) != NULL) {
		if (*line_a == '\n') {
			; // skip the empty lines...
		}
		else {
			getPattern(line_a, pattern_a1, '\t', icol_a1);
			getPattern(line_a, pattern_a2, '\t', icol_a2);
			strcat(pattern_a1, pattern_a2);
			trieIncert(pattern_a1, &root_a);
		}
	}
	// search and generate target files A&B_B and B-A
	while (fgets(line_b, LINE_BUFFER, ptr_b) != NULL) {
		if (*line_b == '\n') {
			; // skip the empty lines
		}
		else {
			getPattern(line_b, pattern_b1, '\t', icol_b1);
			getPattern(line_b, pattern_b2, '\t', icol_b2);
			strcat(pattern_b1, pattern_b2);
			isMatch = trieSearch(pattern_b1, root_a);
			if (isMatch) {
				fprintf(ptr_abb, "%s", line_b);
			}
			else {
				fprintf(ptr_b_a, "%s", line_b);
			}
			trieIncert(pattern_b1, &root_b);
		}
	}
	free(root_a);
	fseek(ptr_a, 0, SEEK_SET); // move the pointer to the start
	// search and generate target files A&B_A and A-B
	while (fgets(line_a, LINE_BUFFER, ptr_a) != NULL) {
		if (*line_a == '\n') {
			; // skip the empty lines
		}
		else {
			getPattern(line_a, pattern_a1, '\t', icol_a1);
			getPattern(line_a, pattern_a2, '\t', icol_a2);
			strcat(pattern_a1, pattern_a2);
			isMatch = trieSearch(pattern_a1, root_b);
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
