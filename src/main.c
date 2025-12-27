#include "../include/vector.h"
#include <stdio.h>

void print_vector_info(Vector *v) {
  if (v == NULL) {
    printf("Vector does not exists!!!");
    return;
  }
  printf("Vector size: %lu \n", vector_size(v));
  printf("Vector capacity: %lu \n", vector_capacity(v));
  for (int i = 0; i < vector_size(v); i++) {
    int elem;
    vector_get(v, i, &elem);
    printf("vector[%d] = %d \n", i, elem);
  }
  printf("-------------------------\n");
}

int main() {
  Vector *v = vector_create(sizeof(int));
  vector_reserve(v, 4);
  print_vector_info(v);

  for (int i = 0; i < 10; i++)
      vector_push_back(v, &i);
  print_vector_info(v);


  return 0;
}
