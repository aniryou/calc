#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"

#define BOPEN '('
#define BCLOSE ')'
#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
#define NUM 0

#define SUCCESS 1
#define ERR 0

struct opnode {
	int type;		/* one of above types */
	float value;	/* for NUM, float value */
};

struct opnode *createOpNode(int type) {
	struct opnode *n = (struct opnode *) malloc(sizeof(struct opnode));
	n->type = type;
	return n;
}

/* apply operation on top 2 elements of stack
   push result back to stack
   return ERR / SUCCESS as status code
 */
int apply(Stack *s_num, int op) {
	struct opnode *node;
	float val1, val2;

	if((node=(struct opnode *)pop(s_num)) == NULL)
		return ERR;

	val2 = node->value; free(node);

	if((node=(struct opnode *) pop(s_num)) == NULL)
		return ERR;

	val1 = node->value; free(node);
	
	switch(op) {
		case ADD:
			val1 = val1 + val2;
			break;
		case SUB:
			val1 = val1 - val2;
			break;
		case MUL:
			val1 = val1 * val2;
			break;
		case DIV:
			val1 = val1 / val2;
			break;
		default:
			return ERR;
	}

	node = createOpNode(NUM); node->value = val1;
	push(s_num, (void *) node);

	return SUCCESS;
}

/*  */
int parse(char *s, float *f) {
	Stack *s_num = createStack();	/* operand stack */
	Stack *s_op = createStack();	/* operator stack */
	
	struct opnode *node;	
	char op, *tmpstr;
	float val1, val2;

	while(*s) {
		switch(*s) {
			case ' ': case '\t':
				/* skip */
				s++;
				break;
			case BOPEN:
				/* push to opstack */
				push(s_op, (void *)createOpNode(BOPEN));
				s++;
				break;
			case BCLOSE:
				/* process opstack, uptill corresponding BOPEN */
				if(isEmpty(s_op))
					return ERR;
				while((node=(struct opnode *) pop(s_op))!=NULL && (op=node->type)!=BOPEN) {
					free(node);
					if(!apply(s_num, op))
						return ERR;		
				}
				s++;
				break;
			case MUL: case DIV:
				/* push to opstack */
				push(s_op, (void *)createOpNode(*s));
				s++;
				break;
			case ADD: case SUB:
				/* process any MUL/DIV on top of opstack before pushing to opstack */
				while((node=pop(s_op))!=NULL && ((op=node->type)==MUL || op==DIV)) {
					free(node);
					if(!apply(s_num, op))
						return ERR;
				}
				if(node)
					push(s_op, (void *)node);
				push(s_op, (void *)createOpNode(*s));
				s++;
				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				/* move till end of number, parse float and push to numstack */
				tmpstr = s;
				while(isdigit(*s))
					s++;
				if(*s=='.') {
					while(isdigit(*s))
						s++;
				}
				node = createOpNode(NUM);
				node->value = atof(tmpstr);
				push(s_num, (void *)node);
				break;
			default:
				return ERR;
		}
	}

	/* process opstack */
	while((node=(struct opnode *) pop(s_op))!=NULL) {
		op = node->type;
		free(node);
		if(!apply(s_num, op))
			return ERR;
	}

	/* numstack has result */
	if(isEmpty(s_num))
		return ERR;

	node = (struct opnode *) pop(s_num); *f = node->value; free(node);
	return SUCCESS;
}

int main(int argc, char **argv) {
	float f;
	if(argc!=2) {
		printf("Usage: %s \"<expression>\"\n", argv[1]);
		return ERR;
	}
	if(!parse(argv[1], &f)) {
		printf("Exception occurred\n");
		return ERR;
	}
	printf("Output: %.2f\n", f);
	return SUCCESS;
}