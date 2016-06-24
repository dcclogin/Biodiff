#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliary.h"

/* usage: tell the user some usages and errors of the problem */
void usage(int option) {
	switch (option) {
		case 0:
			printf("===================== Welcome to use biodiff! =================\n");
			printf("  1. Usage: biodiff -c -a ca1,ca2 -b cb1,cb2 file_A file_B     \n");
			printf("  2. Usage: biodiff -n -a ca -b cb file_A file_B [\"] [\"]     \n");
			printf("---------------------------------------------------------------\n");
			printf("  > * [-c] refers to coordinate-based comparation;             \n");
			printf("  > * [-n] refers to name-based comparation;                   \n");
			printf("  > * ca1/ca2/cb1/cb2/ca/cb refer to columns you select;       \n");
			printf("  > * In [-n] mode you onlu need to select one column;         \n");
			printf("  > * In [-c] mode 2 columns separated by ',' are required;    \n");
			printf("  > * In [-n] mode one or two \" are optional (default: tab);  \n");
			printf("  > * In [-n] the first separator is linked to file_A;         \n");
			printf("  > * In [-n] the second separator is linked to file_B;        \n");
			printf("  > * Here 'overlap' is defined as 'totally equal';            \n");
			printf("===============================================================\n");
			break;
		case 1:
			printf("============================================================\n");
			printf("  1. Usage: biodiff -c -a ca1,ca2 -b cb1,cb2 file_A file_B  \n");
			printf("  2. Usage: biodiff -n -a ca -b cb file_A file_B [\"] [\"]  \n");
			printf("============================================================\n");
			break;
		case 2:
			printf("===========================================================\n");
			printf("  Error: only one number following [-a] and [-b] is valid  \n");
			printf("===========================================================\n");
			break;
		case 3:
			printf("=========================================================\n");
			printf("  Error: cannot open file_A, please check the file name  \n");
			printf("=========================================================\n");
			break;
		case 4:
			printf("=========================================================\n");
			printf("  Error: cannot open file_B, please check the file name  \n");
			printf("=========================================================\n");
			break;
		case 5:
			printf("==============================================================\n");
			printf("  Error: cannot generate output files due to some reasons...  \n");
			printf("==============================================================\n");
			break;
		case 6:
			printf("============================================================================\n");
			printf("  Error: you should select 2 columns (ex. 3,4), and their order does count  \n");
			printf("  Warning: only first 2 columns will be calculated if you select 3 or more  \n");
			printf("============================================================================\n");
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
void getPattern(char *line, char *pattern, char token, int c) {
	int i = 1;
	char *pIn = line;
    char *pOut = pattern;
    if (c < 1) {
        *pattern = '\0';
    }
    while (*pIn != '\0' && i < c) {
        if (*pIn == token) {
            pIn++;
            i++;
        }
        else {
            pIn++;
        }
    }
    for (; *pIn != '\0' && *pIn != token; pIn++, pOut++) {
    	*pOut = *pIn;
    }
    *pOut = '\0';
}
