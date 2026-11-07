#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct RingBuffer RingBuffer;

/**
 * Creates a ring buffer.
 *
 * @param capacity Maximum number of elements.
 * @param elem_size Size of each element in bytes.
 * @return Pointer to the buffer or NULL on failure.
 */
RingBuffer *ring_buffer_create(size_t capacity, size_t elem_size);

/**
 * Destroys the ring buffer and frees allocated memory.
 *
 * REVIEW: src/ring-buffer.c uses the same RingBuffer ** signature, but its
 * body currently does free(rb); rb = NULL; which frees the pointer-to-pointer
 * and never nulls the caller's pointer. Use free(*rb); *rb = NULL; instead.
 */
void ring_buffer_destroy(RingBuffer **rb);

/**
 * Adds an element to the buffer.
 *
 * @return true on success, false if full or invalid.
 */
bool ring_buffer_push(RingBuffer *rb, const void *value);

/**
 * Retrieves and removes the oldest element.
 *
 * @param out Pointer to memory where the element will be copied.
 * @return true on success, false if empty or invalid.
 */
bool ring_buffer_pop(RingBuffer *rb, void *out);

/**
 * Retrieves the oldest element without removing it.
 *
 * @param out Pointer to memory where the element will be copied.
 * @return true on success, false if empty or invalid.
 */
bool ring_buffer_peek(const RingBuffer *rb, void *out);

/**
 * Checks whether the buffer is empty.
 */
bool ring_buffer_is_empty(const RingBuffer *rb);

/**
 * Checks whether the buffer is full.
 */
bool ring_buffer_is_full(const RingBuffer *rb);

/**
 * Removes all elements from the buffer.
 */
void ring_buffer_clear(RingBuffer *rb);

/*
 * REVIEW: peek, is_empty, size, capacity, and clear are declared here but
 * not implemented in src/ring-buffer.c yet.
 */

#endif // RING_BUFFER_H
