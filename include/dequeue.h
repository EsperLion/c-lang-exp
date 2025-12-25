#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>

typedef struct Deque Deque;

/* Create empty deque */
Deque *deque_create(void);

/* Free deque memory */
void deque_destroy(Deque *d);

/* Insert element at front */
int deque_push_front(Deque *d, int value);

/* Insert element at back */
int deque_push_back(Deque *d, int value);

/* Remove element from front */
int deque_pop_front(Deque *d, int *out);

/* Remove element from back */
int deque_pop_back(Deque *d, int *out);

/* Return number of elements */
size_t deque_size(const Deque *d);

/* Check if deque is empty */
int deque_is_empty(const Deque *d);

/* Remove all elements */
void deque_clear(Deque *d);

#endif
