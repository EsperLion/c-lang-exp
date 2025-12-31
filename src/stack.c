#include "../include/stack.h"
#include "../include/vector.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct Stack {
  Vector *vector;
} Stack;

Stack *stack_create(size_t elem_size) {
  Stack *stack = malloc(sizeof(Stack));
  if (!stack)
    return 0;

  stack->vector = vector_create(elem_size);

  if (!stack->vector) {
    free(stack);
    return 0;
  }

  return stack;
}

void stack_destroy(Stack **stack) {
  if (!stack || !*stack)
    return;

  vector_destroy(&(*stack)->vector);
  (*stack)->vector = NULL;
  free(*stack);
  *stack = NULL;
}

int stack_push(Stack *stack, const void *value) {
  if (!stack || !value) {
    return 0;
  }

  return vector_push_back(stack->vector, value);
}

int stack_pop(Stack *stack, void *out) {
  if (!stack) {
    return 0;
  }

  return vector_pop_back(stack->vector, out);
}

int stack_peek(const Stack *stack, void *out) {
  if (!stack || !out) {
    return 0;
  }

  size_t v_size = vector_size(stack->vector);
  if (v_size == 0)
      return 0;

  return vector_get(stack->vector, v_size - 1, out);
}

size_t stack_size(const Stack *stack) {
  if (!stack) {
    return 0;
  }

  return vector_size(stack->vector);
}

int stack_is_empty(const Stack *stack) {
  if (!stack) {
    return 0;
  }

  return !vector_size(stack->vector);
}
