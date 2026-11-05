#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Queue Queue;

/**
 * Create a queue for elements of fixed size.
 *
 * Example:
 *   Queue *q = queue_create(sizeof(int));
 */
Queue *queue_create(size_t elem_size);

/**
 * Destroy queue and free all allocated memory.
 * After this call, *queue is set to NULL.
 */
void queue_destroy(Queue **queue);

/**
 * Add an element to the end of the queue.
 *
 * Copies elem_size bytes from value into the queue.
 *
 * Returns 1 on success, 0 on failure.
 */
int queue_enqueue(Queue *queue, const void *value);

/**
 * Remove and return the element from the front of the queue.
 *
 * Copies the element into out.
 * If out is NULL, element is just removed.
 *
 * Returns 1 on success, 0 if queue is empty or invalid.
 */
int queue_dequeue(Queue *queue, void *out);

/**
 * Get the element at the front of the queue without removing it.
 *
 * Copies the element into out.
 *
 * Returns 1 on success, 0 if queue is empty or invalid.
 */
int queue_peek(const Queue *queue, void *out);

/**
 * Get number of elements in the queue.
 */
size_t queue_size(const Queue *queue);

/**
 * Check whether the queue is empty.
 *
 * Returns 1 if empty, 0 otherwise.
 */
int queue_is_empty(const Queue *queue);

#endif /* QUEUE_H */
