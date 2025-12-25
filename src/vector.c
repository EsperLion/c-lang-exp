#include "../include/vector.h"
#include <stdlib.h>

typedef struct Vector {
  int *elements;
  size_t size;
  size_t capacity;
} Vector;

Vector *vector_create(void) {
  Vector *vector = malloc(sizeof(Vector));
  vector->elements = NULL;
  vector->size = 0;
  vector->capacity = 0;

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

size_t vector_size(const Vector *v) { return v->size; }

size_t vector_capacity(const Vector *v) { return v->capacity; }

int vector_reserve(Vector *v, size_t new_capacity) {
  if (v == NULL)
    return 0;

  if (v->capacity == 0) {
    v->elements = malloc(new_capacity * sizeof(int));
    return 1;
  }

  if (new_capacity <= v->capacity)
    return 1;

  int *new_elements = malloc(new_capacity * sizeof(int));
  if (new_elements == NULL)
    return 0;

  for (int i = 0; i < v->size; i++) {
    *(new_elements + i) = *(v->elements + i);
  }

  free(v->elements);
  v->elements = new_elements;
  v->capacity = new_capacity;

  return 1;
}

int vector_get(const Vector *v, size_t index, int *out) {
  if (index > v->size) {
    return 0;
  }
  *out = v->elements[index];
  return 1;
}

int vector_set(Vector *v, size_t index, int value) {
  if (index >= v->size || v == NULL) {
    return 0;
  }

  v->elements[index] = value;
  return 1;
}

int vector_push_back(Vector *v, int value) {
  if (v == NULL)
    return 0;
  if (v->size == v->capacity) {
    vector_reserve(v, v->capacity * 2);
  }
  v->elements[v->size++] = value;

  return 1;
}

int vector_pop_back(Vector *v, int *out) {
  if (v == NULL)
    return 0;
  if (v->size == 0)
    return 0;

  *out = v->elements[v->size--];

  return 1;
}

void vector_clear(Vector *v) {
  for (int i = 0; i < v->size; i++) {
    v->elements[i] = 0;
  }

  v->size = 0;
}
