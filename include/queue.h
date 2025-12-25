#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Queue Queue;

/* Create empty queue */
Queue *queue_create(void);

/* Free queue memory */
void queue_destroy(Queue *q);

/* Add element to back */
int queue_enqueue(Queue *q, int value);

/* Remove element from front */
int queue_dequeue(Queue *q, int *out);

/* Read front element without removing */
int queue_peek(const Queue *q, int *out);

/* Return number of elements */
size_t queue_size(const Queue *q);

/* Check if queue is empty */
int queue_is_empty(const Queue *q);

/* Remove all elements */
void queue_clear(Queue *q);

#endif
