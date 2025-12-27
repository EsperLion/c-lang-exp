#include "../include/vector.h"
#include <stdlib.h>
#include <string.h>

typedef struct Vector {
  void *elements;
  size_t size;
  size_t capacity;
  size_t elem_size;
} Vector;

Vector *vector_create(size_t elem_size) {
  Vector *vector = malloc(sizeof(Vector));
  vector->elements = NULL;
  vector->size = 0;
  vector->capacity = 0;
  vector->elem_size = elem_size;

  return vector;
}

void vector_destroy(Vector **v) {
  if (v == NULL || *v == NULL)
    return;
  free((*v)->elements);
  (*v)->elements = NULL;
  free(*v);
  *v = NULL;
}

size_t vector_size(const Vector *v) {
  if (v == NULL)
    return 0;
  return v->size;
}

size_t vector_capacity(const Vector *v) {
  if (v == NULL)
    return 0;
  return v->capacity;
}

int vector_reserve(Vector *v, size_t new_capacity) {
  if (v == NULL)
    return 0;

  if (new_capacity <= v->capacity)
    return 1;

  void *tmp = realloc(v->elements, new_capacity * v->elem_size);
  if (!tmp)
    return 0;

  v->elements = tmp;
  v->capacity = new_capacity;
  return 1;
}

int vector_get(const Vector *v, size_t index, void *out) {
  if (v == NULL || out == NULL || index >= v->size) {
    return 0;
  }
  memcpy(out, (char *)v->elements + index * v->elem_size, v->elem_size);
  return 1;
}

int vector_set(Vector *v, size_t index, const void *value) {
  if (v == NULL || index >= v->size) {
    return 0;
  }
  memcpy((char *)v->elements + index * v->elem_size, value, v->elem_size);
  return 1;
}

int vector_push_back(Vector *v, const void *value) {
  if (v == NULL)
    return 0;
  if (v->size == v->capacity) {
    vector_reserve(v, v->capacity ? v->capacity * 2 : 1);
  }
  memcpy((char *)v->elements + v->size * v->elem_size, value, v->elem_size);
  v->size++;

  return 1;
}

int vector_pop_back(Vector *v, void *out) {
  if (v == NULL)
    return 0;
  if (v->size == 0)
    return 0;

  memcpy(out, (char *)v->elements + (v->size - 1) * v->elem_size, v->elem_size);

  v->size--;

  return 1;
}

void vector_clear(Vector *v) { v->size = 0; }
