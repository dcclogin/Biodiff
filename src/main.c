#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "cdiff.h"
#include "ndiff.h"

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
		printf("Open input files: success!\n");
		// set buffer for the input streams
		setvbuf(ptr_a, NULL, _IOFBF, FILE_BUFFER);
		setvbuf(ptr_b, NULL, _IOFBF, FILE_BUFFER);
		// create empty output files
		FILE *ptr_aba = fopen("A_and_B_A", "w");
		FILE *ptr_abb = fopen("A_and_B_B", "w");
		FILE *ptr_a_b = fopen("Only_in_A", "w");
		FILE *ptr_b_a = fopen("Only_in_B", "w");
		if ((ptr_aba == NULL) || (ptr_abb == NULL) || 
			(ptr_a_b == NULL) || (ptr_b_a == NULL)) {
			usage(5);
			exit(EXIT_FAILURE);
		}
		printf("Generate output files: success!\n");
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
			if (isInteger(col_a1) && isInteger(col_a2) &&
				isInteger(col_b1) && isInteger(col_b2) &&
				strstr(A2, ",") && strstr(A4, ",")) {
				// into the coordinate-based diff 
				CDIFF();
				printf("Coordinate-based diff: done!\n");
			}
			else {
				usage(6);
				exit(EXIT_FAILURE);
			}
		}
		/* [-n] mode: name-based diff */
		else if (A01 == 'n') {
			if (isInteger(A2) == 1 && isInteger(A4) == 1) {
				char token_a = '\t';
				char token_b = '\t';
				if (argc == 10) {
					if (A70 == '"') {
						token_a = A70;
					}
					if (A80 == '"') {
						token_b = A80;
					}
				}
				// into the name-based diff
				NDIFF();
				printf("Name-based diff: done!\n");
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
	printf("Please check the output files...\n");
	return 0;
}