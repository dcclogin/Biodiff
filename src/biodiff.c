/*
* @Author: rumia
* @Date:   2016-08-08 23:23:45
* @Last Modified by:   rumia
* @Last Modified time: 2016-08-11 03:42:53
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FILE_BUFFER 1024
#define LINE_BUFFER 512
#define PATTERN_SIZE 256
#define BRANCH_SIZE 128
#define A00 **argv
#define APP **++argv
#define A01 *(*argv+1)
#define A10 *(*(argv+1)+0)
#define A11 *(*(argv+1)+1)
#define A30 *(*(argv+3)+0)
#define A31 *(*(argv+3)+1)
#define A70 *(*(argv+7)+0)
#define A80 *(*(argv+8)+0)
#define A2 *(argv+2)
#define A4 *(argv+4)
#define A5 *(argv+5)
#define A6 *(argv+6)

#define CDIFF() cdiff(A2, A4, ptr_a, ptr_b, ptr_aba, ptr_abb, ptr_a_b, ptr_b_a)
#define NDIFF() ndiff(atoi(A2), atoi(A4), token_a, token_b, ptr_a, ptr_b, ptr_aba, ptr_abb, ptr_a_b, ptr_b_a)


typedef struct Trienode {
	int isPattern; // 1 if this node represent for a pattern; 0 otherwise
	struct Trienode *next[BRANCH_SIZE];
} Retrieval;

/*typedef struct ACnode {
	int nCount;
	struct ACnode *parent;
	struct ACnode *failure;
	struct ACnode *success[BRANCH_SIZE];
} ACnode;*/


void usage(int);
int isInteger(char *);
char *getPattern(char *, char *, char, int c);
void cdiff(char *, char *, FILE *, FILE *, FILE *, FILE *, FILE *, FILE *);
void ndiff(int, int, char, char, FILE *, FILE *, FILE *, FILE *, FILE *, FILE *);
Retrieval *trieGenerate(void);
void trieIncert(char *, Retrieval **);
int trieSearch(char *, Retrieval *);


int main(int argc, char **argv) {
	if ((argc != 8) && (argc != 10)) {
		if (argc == 1) {
			usage(0);
			exit(EXIT_FAILURE);
		}
		else {
			usage(1);
			exit(EXIT_FAILURE);
		}
	}
	else {
		if (APP == '-' &&
			A10 == '-' &&
			A11 == 'a' &&
			A30 == '-' &&
			A31 == 'b') {
			// open the input files
			FILE *ptr_a = fopen(A5, "r");
			FILE *ptr_b = fopen(A6, "r");
			if (ptr_a == NULL) {
				usage(3);
				exit(EXIT_FAILURE);
			}
			if (ptr_b == NULL) {
				usage(4);
				exit(EXIT_FAILURE);
			}
			// set buffer for the input streams
			setvbuf(ptr_a, NULL, _IOFBF, FILE_BUFFER);
			setvbuf(ptr_b, NULL, _IOFBF, FILE_BUFFER);
			// create empty output files
			FILE *ptr_aba = fopen("A&B_A", "w");
			FILE *ptr_abb = fopen("A&B_B", "w");
			FILE *ptr_a_b = fopen("A-B", "w");
			FILE *ptr_b_a = fopen("B-A", "w");
			if ((ptr_aba == NULL) ||
				(ptr_abb == NULL) ||
				(ptr_a_b == NULL) ||
				(ptr_b_a == NULL)) {
				usage(5);
				exit(EXIT_FAILURE);
			}
			// set buffer for the output streams
			setvbuf(ptr_aba, NULL, _IOFBF, FILE_BUFFER);
			setvbuf(ptr_abb, NULL, _IOFBF, FILE_BUFFER);
			setvbuf(ptr_a_b, NULL, _IOFBF, FILE_BUFFER);
			setvbuf(ptr_b_a, NULL, _IOFBF, FILE_BUFFER);
			/* [-c] mode: coordinate-based diff */
			if (A01 == 'c') {
				char col_a1[5], col_a2[5];
				char col_b1[5], col_b2[5];
				getPattern(A2, col_a1, ',', 1);
				getPattern(A2, col_a2, ',', 2);
				getPattern(A4, col_b1, ',', 1);
				getPattern(A4, col_b2, ',', 2);
				if (isInteger(col_a1) &&
					isInteger(col_a2) &&
					isInteger(col_b1) &&
					isInteger(col_b2) &&
					strstr(A2, ",") &&
					strstr(A4, ",")) {
					// into the coordinate-based diff
					CDIFF();
				}
				else {
					usage(6);
					exit(EXIT_FAILURE);
				}
			}
			/* [-n] mode: name-based diff */
			else if (A01 == 'n') {
				if (isInteger(A2) == 1 &&
					isInteger(A4) == 1) {
					char token_a = '\t';
					char token_b = '\t';
					if (argc == 10)
					{
						if (A70 == '"') {
							token_a = A70;
						}
						if (A80 == '"') {
							token_b = A80;
						}
					}
					// into the name-based diff
					NDIFF();
				}
				else {
					usage(2);
					exit(EXIT_FAILURE);
				}
			}
			else {
				usage(1);
				exit(EXIT_FAILURE);
			}
			// close all streams
			fclose(ptr_a);
			fclose(ptr_b);
			fclose(ptr_aba);
			fclose(ptr_abb);
			fclose(ptr_a_b);
			fclose(ptr_b_a);
		}
		else {
			usage(1);
			exit(EXIT_FAILURE);
		}
	}
	printf("Comparation complete!\n");
	return 0;
}

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
	// should pattern_a, pattern_a1, pattern_a2 be initialized here? if should, how?
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

/* usage: tell the user some usages and errors of the problem */
void usage(int option) {
	switch (option) {
		case 0:
			printf("===================== Welcome to use biodiff! =====================\n");
			printf("  1. Usage: biodiff -c -a ca1,ca2 -b cb1,cb2 file_A file_B 		   \n");
			printf("  2. Usage: biodiff -n -a ca -b cb file_A file_B [sprt] [sprt]     \n");
			printf("-------------------------------------------------------------------\n");
			printf("  > * [-c] refers to coordinate-based comparation;                 \n");
			printf("  > * [-n] refers to name-based comparation;                       \n");
			printf("  > * ca1/ca2/cb1/cb2/ca/cb refer to columns you select;           \n");
			printf("  > * In [-n] mode you onlu need to select one column;             \n");
			printf("  > * In [-c] mode 2 columns separated by ',' are required;        \n");
			printf("  > * In [-n] mode two separators are optional (default: tab);     \n");
			printf("  > * In [-n] the first separator is linked to file_A;             \n");
			printf("  > * In [-n] the second separator is linked to file_B;            \n");
			printf("  > * Here 'overlap' is defined as 'totally equal';                \n");
			printf("===================================================================\n");
			break;
		case 1:
			printf("Usage: biodiff [-c/-n] -a [numbers,] -b [numbers,] file_A file_B\n");
			break;
		case 2:
			printf("Error: only a single number following [-a] and [-b] is valid\n");
			break;
		case 3:
			printf("Error: cannot open file_A, please check the file name\n");
			break;
		case 4:
			printf("Error: cannot open file_B, please check the file name\n");
			break;
		case 5:
			printf("Error: cannot generate output files due to some reasons...\n");
			break;
		case 6:
			printf("Error: you should select 2 columns (ex. 3,4), and their order does counts\n");
			printf("Warning: only first 2 columns will be calculated if you select 3 or more (ex. 3,4,5)\n");
			break;
		default:
			;
	}
}

/* isNumber: return 1 if a string is actually a 'number' and 0 otherwise */
int isInteger(char *str){
	int i;
	int flag = 1;
	if (*str == '\0') {
		return 0;
	}
	for (i = 0; i < strlen(str); i++){
		if (isdigit(*(str+i)) == 0){
			flag = 0;
			return 0;
			break;
		}
	}
	if (flag == 1) {
		return 1;
	}
}

/* getPattern: get a specific column according to c */
char *getPattern(char *line, char *pattern, char token, int c) {
	char *pIn = line;
    char *pOut = pattern;
    int i = 1;
    if (c < 1) {
        *pattern = 0;
        return pattern;
    }
    for (; *pIn != 0 && *pIn == token; pIn++);
    while (*pIn !=0 && i < c) {
        if (*pIn == token) {
            for (; *pIn != 0 && *pIn == token; pIn++);
            i++;
        }
        else {
            pIn++;
        }
    }
    for (; *pIn != 0 && *pIn != token; pIn++, pOut++) *pOut = *pIn;
    *pOut = 0;
    return pOut;
}

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
			;
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
	Retrieval * temp = root;
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
