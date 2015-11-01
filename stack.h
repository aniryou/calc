#include <stdlib.h>

struct snode {
	void *data;
	struct snode *next;
};

struct snode *createSNode(void *data);

typedef struct {
	struct snode *top;
} Stack;

Stack *createStack();

void push(Stack *s, void *data);
void *pop(Stack *s);
int isEmpty(Stack *s);
