#include <stdio.h>
#include "stack.h"

struct snode *createSNode(void *data) {
	struct snode *n = (struct snode *) malloc(sizeof(struct snode));
	n->data = data;
	n->next = NULL;
	return n;
}

Stack *createStack() {
	Stack *s = (Stack *) malloc(sizeof(Stack));
	s->top = NULL;
	return s;
}

void push(Stack *s, void *data) {
	struct snode *n = createSNode(data);
	if(!s->top)
		s->top = n;
	else {
		n->next = s->top;
		s->top = n;
	}
}

void *pop(Stack *s) {
	struct snode *n = s->top;
	void *data = NULL;
	if(n) {
		s->top = s->top->next;
		data = n->data;
		free(n);
	}
	return data;
}

int isEmpty(Stack *s) {
	return s->top == NULL;
}