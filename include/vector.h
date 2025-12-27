#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct Vector Vector;

/* Create empty vector */
Vector *vector_create(size_t s);

/* Free vector and its internal memory */
void vector_destroy(Vector **v);

/* Return number of stored elements */
size_t vector_size(const Vector *v);

/* Return allocated capacity */
size_t vector_capacity(const Vector *v);

/* Ensure capacity is at least given value */
int vector_reserve(Vector *v, size_t capacity);

/* Get element by index, store into out */
int vector_get(const Vector *v, size_t index, void *out);
/* Set element at index to value */
int vector_set(Vector *v, size_t index, const void *value);

/* Append element to the end */
int vector_push_back(Vector *v, const void *value);

/* Remove last element and return it */
int vector_pop_back(Vector *v, void *out);

/* Remove all elements, keep capacity */
void vector_clear(Vector *v);

#endif
