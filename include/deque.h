#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h> /* size_t */

/*
 * Deque (double-ended queue) implemented as a growable ring buffer.
 *
 * Elements are stored by value: each push copies elem_size bytes.
 *
 *   Deque *d = deque_create(sizeof(int));
 *
 * All functions that return int return 1 on success and 0 on failure
 * (invalid argument, empty deque, or allocation failure).
 */
typedef struct Deque Deque;

/* Create an empty deque for elements of elem_size bytes.
 * Returns NULL on invalid elem_size or allocation failure. */
Deque *deque_create(size_t elem_size);

/* Free the deque and its internal storage.
 * After this call, *deque is set to NULL. */
void deque_destroy(Deque **deque);

/* Number of stored elements. Returns 0 for NULL. */
size_t deque_size(const Deque *deque);

/* Allocated capacity. Returns 0 for NULL. */
size_t deque_capacity(const Deque *deque);

/* Returns 1 if the deque is empty, 0 otherwise or for NULL. */
int deque_is_empty(const Deque *deque);

/* Ensure capacity is at least `capacity` elements.
 * Existing elements are preserved. */
int deque_reserve(Deque *deque, size_t capacity);

/* Add an element to the front. */
int deque_push_front(Deque *deque, const void *value);

/* Add an element to the back. */
int deque_push_back(Deque *deque, const void *value);

/* Remove the front element and copy it into out.
 * out may be NULL if the value is not needed. */
int deque_pop_front(Deque *deque, void *out);

/* Remove the back element and copy it into out.
 * out may be NULL if the value is not needed. */
int deque_pop_back(Deque *deque, void *out);

/* Copy the front element into out without removing it. */
int deque_front(const Deque *deque, void *out);

/* Copy the back element into out without removing it. */
int deque_back(const Deque *deque, void *out);

/* Copy the element at logical index (0 .. size-1) into out. */
int deque_get(const Deque *deque, size_t index, void *out);

/* Replace the element at logical index (0 .. size-1) with value. */
int deque_set(Deque *deque, size_t index, const void *value);

/* Remove all elements, keeping allocated capacity. */
void deque_clear(Deque *deque);

#endif /* DEQUE_H */
