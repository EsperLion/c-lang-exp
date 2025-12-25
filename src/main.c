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
  Vector *vec = vector_create();
  vector_reserve(vec, 3);
  print_vector_info(vec);

  vector_push_back(vec, 28);
  vector_push_back(vec, 7);
  vector_push_back(vec, 1995);
  print_vector_info(vec);

  int pop_val;
  vector_pop_back(vec, &pop_val);
  print_vector_info(vec);
  vector_push_back(vec, 100);
  vector_push_back(vec, 101);
  vector_push_back(vec, 102);
  vector_push_back(vec, 103);
  vector_push_back(vec, 104);
  vector_push_back(vec, 105);
  print_vector_info(vec);

  vector_clear(vec);
  print_vector_info(vec);

  vector_destroy(&vec);
  print_vector_info(vec);

  return 0;
}
