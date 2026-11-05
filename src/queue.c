#include "../include/queue.h"
#include "../include/list.h"
#include <stdlib.h>

typedef struct Queue {
  List *list;
} Queue;

Queue *queue_create(size_t elem_size) {
    Queue *q = malloc(sizeof(Queue));
    if (!q) {
        return 0;
    }
    q->list = list_create(elem_size);
    if (!q->list) {
        free(q);
        return 0;
    }
    return q;
}


void queue_destroy(Queue **queue) {
    if (!queue || !*queue) {
        return;
    }

    list_destroy(&(*queue)->list);
    (*queue)->list = NULL;
    free(*queue);
    *queue = NULL;
    return;
}

int queue_enqueue(Queue *queue, const void *value) {
    if (!queue || !value)
        return 0;

    return list_push_back(queue->list, value);
}

int queue_dequeue(Queue *queue, void *out) {
    if (!queue)
        return 0;

    return list_pop_front(queue->list, out);
}

int queue_peek(const Queue *queue, void *out) {
    if (!queue)
        return 0;

    return list_get(queue->list, 0, out);
}

size_t queue_size(const Queue *queue) {
    if (!queue)
        return 0;

    return list_size(queue->list);
}

int queue_is_empty(const Queue *queue) {
    if (!queue)
        return 0;

    return  list_is_empty(queue->list);
}
