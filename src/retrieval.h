#define BRANCH_SIZE 128

typedef struct Trienode {
	int isPattern; // 1 if this node represent for a pattern; 0 otherwise
	struct Trienode *next[BRANCH_SIZE];
} Retrieval;

Retrieval *trieGenerate(void);
void trieIncert(char *, Retrieval **);
int trieSearch(char *, Retrieval *);