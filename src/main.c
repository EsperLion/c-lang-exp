#include "../include/vector.h"
#include "../include/list.h"
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

typedef struct Person {
    char *name;
    unsigned char age;
} Person;

void print_list_data(void *p) {
    Person *per = (Person*)p;
    printf("Name: %s, Age: %d \n", per->name, per->age);
}

void test_list () {
    List *list = list_create(sizeof(Person));
    Person p1 = {
        .name = "German",
        .age = 30,
    };
    Person p2 = {
        .name = "Nastya",
        .age = 30,
    };
    Person p3 = {
        .name = "hanna",
        .age = 50,
    };

    list_push_back(list, &p1);
    list_push_front(list, &p2);
    list_push_back(list, &p3);
    list_foreach(list, &print_list_data);
    printf("-------------------------\n");

    Person outP2, outP3;
    list_pop_front(list, &outP2);
    list_pop_back(list, &outP3);

    list_foreach(list, &print_list_data);
    printf("Delete with pop front (name:%s, age: %d) \n", outP2.name, outP2.age);
    printf("Delete with pop back (name:%s, age: %d) \n", outP3.name, outP3.age);
    printf("-------------------------\n");

    list_set(list, 0, &p2);
    list_foreach(list, &print_list_data);
    printf("-------------------------\n");

    list_remove_at(list, 0);
    list_foreach(list, &print_list_data);
    printf("-------------------------\n");

    list_push_back(list, &p1);
    list_push_back(list, &p2);
    list_push_back(list, &p3);
    Person get;
    list_get(list, 1, &get);
    list_foreach(list, &print_list_data);
    printf("Get with list_get (name:%s, age: %d) \n", get.name, get.age);
    printf("-------------------------\n");

    list_clear(list);
    list_foreach(list, &print_list_data);
    printf("-------------------------\n");
}

int main() {
  // Vector *v = vector_create(sizeof(int));
  // vector_reserve(v, 4);
  // print_vector_info(v);

  // for (int i = 0; i < 10; i++)
  //     vector_push_back(v, &i);
  // print_vector_info(v);
  //
  //
  test_list();


  return 0;
}
