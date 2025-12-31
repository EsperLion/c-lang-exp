#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct Stack Stack;

/**
 * Create a stack for elements of fixed size.
 *
 * Example:
 *   Stack *s = stack_create(sizeof(int));
 */
Stack *stack_create(size_t elem_size);

/**
 * Destroy stack and free all allocated memory.
 * After this call, *stack is set to NULL.
 */
void stack_destroy(Stack **stack);

/**
 * Push element to the top of the stack.
 *
 * Copies elem_size bytes from value into the stack.
 *
 * Returns 1 on success, 0 on failure.
 */
int stack_push(Stack *stack, const void *value);

/**
 * Remove and return the top element of the stack.
 *
 * Copies the top element into out.
 * If out is NULL, element is just removed.
 *
 * Returns 1 on success, 0 if stack is empty or invalid.
 */
int stack_pop(Stack *stack, void *out);

/**
 * Get the top element without removing it.
 *
 * Copies the top element into out.
 *
 * Returns 1 on success, 0 if stack is empty or invalid.
 */
int stack_peek(const Stack *stack, void *out);

/**
 * Get number of elements in the stack.
 */
size_t stack_size(const Stack *stack);

/**
 * Check whether the stack is empty.
 *
 * Returns 1 if empty, 0 otherwise.
 */
int stack_is_empty(const Stack *stack);

#endif /* STACK_H */
