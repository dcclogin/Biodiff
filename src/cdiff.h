#include <stdio.h>

#define LINE_BUFFER 512
#define PATTERN_SIZE 256

#define CDIFF() cdiff(A2, A4, ptr_a, ptr_b, ptr_aba, ptr_abb, ptr_a_b, ptr_b_a)

void cdiff(char *, char *, FILE *, FILE *, FILE *, FILE *, FILE *, FILE *);
