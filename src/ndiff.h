#include <stdio.h>

#define LINE_BUFFER 512
#define PATTERN_SIZE 256

#define NDIFF() ndiff(atoi(A2), atoi(A4), token_a, token_b, ptr_a, ptr_b, ptr_aba, ptr_abb, ptr_a_b, ptr_b_a)

void ndiff(int, int, char, char, FILE *, FILE *, FILE *, FILE *, FILE *, FILE *);
