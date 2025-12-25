#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct Stack Stack;

/* Create empty stack */
Stack *stack_create(void);

/* Free stack memory */
void stack_destroy(Stack *s);

/* Push value onto stack */
int stack_push(Stack *s, int value);

/* Pop top value from stack */
int stack_pop(Stack *s, int *out);

/* Read top value without removing */
int stack_peek(const Stack *s, int *out);

/* Return number of elements */
size_t stack_size(const Stack *s);

/* Check if stack is empty */
int stack_is_empty(const Stack *s);

/* Remove all elements */
void stack_clear(Stack *s);

#endif
