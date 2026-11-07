#include "../include/ring-buffer.h"
#include <stdlib.h>
#include <string.h>

typedef struct RingBuffer
{
    void *data;
    size_t capacity;
    size_t elem_size;
    /* REVIEW: use size_t to match capacity (unsigned short truncates). */
    unsigned short int head;
    unsigned short int tail;
    size_t size;
} RingBuffer;

RingBuffer *ring_buffer_create(size_t capacity, size_t elem_size)
{
    /* REVIEW: check malloc for NULL, reject zero arguments, guard overflow. */
    RingBuffer *buffer = malloc(sizeof(RingBuffer));

    buffer->data = malloc(capacity * elem_size);
    buffer->capacity = capacity;
    buffer->elem_size = elem_size;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = 0;

    return buffer;
}

void ring_buffer_destroy(RingBuffer **rb)
{
    if (rb == NULL || *rb == NULL)
        return;

    free((*rb)->data);
    (*rb)->data = NULL;
    free(*rb);
    *rb = NULL;
}

bool ring_buffer_push(RingBuffer *rb, const void *value)
{
    memcpy(rb->data + rb->tail * rb->elem_size, value, rb->elem_size);

    rb->tail = (rb->tail + 1) % rb->capacity;

    if (!ring_buffer_is_full(rb))
    {
        rb->size = rb->size + 1;
    } else {
        rb->head = (rb->head + 1) % rb->capacity;
    }

    return true;
}

bool ring_buffer_pop(RingBuffer *rb, void *out) {
    if (ring_buffer_is_empty(rb)) return false;

    memcpy(out, rb->data + rb->head * rb->elem_size, rb->elem_size);
    rb->head = (rb->head + 1) % rb->capacity;
    rb->size--;

    return true;
}

bool ring_buffer_peek(const RingBuffer *rb, void *out) {
    memcpy(out, rb->data + rb->head * rb->elem_size, rb->elem_size);
    return true;
}

bool ring_buffer_is_empty(const RingBuffer *rb) {
    return rb->size == 0;
}

/* REVIEW: header declares const RingBuffer *; this is missing const. */
bool ring_buffer_is_full(const RingBuffer *rb)
{
    return rb != NULL && rb->size == rb->capacity;
}

void ring_buffer_clear(RingBuffer *rb) {
    rb->size = 0;
    rb->tail = 0;
    rb->head = 0;
}