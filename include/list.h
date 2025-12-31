#ifndef LIST_H
#define LIST_H

#include <stddef.h> // size_t

typedef struct ListNode ListNode;

typedef struct List List;

/*
 * creates a list and allocates memory
 *
 *
 *   List *list = list_create(sizeof(int));
 */
List *list_create(size_t elem_size);

/*
 * delete all data from the list and list itself,
 * data passed to the list should be freed by user
 *
 *
 *   list_destroy(&list);
 */
void list_destroy(List **pList);

/*
 *
 *
 *   size_t n = list_size(list);
 */
size_t list_size(const List *list);

/*
 *
 *   if (list_is_empty(list)) { ... }
 */
int list_is_empty(const List *list);

/*
 *
 *   int x = 10;
 *   list_push_front(list, &x);
 */
int list_push_front(List *list, const void *value);

/*
 *
 *   int x = 20;
 *   list_push_back(list, &x);
 */
int list_push_back(List *list, const void *value);

/*
 *
 *   int x;
 *   list_pop_front(list, &x);
 */
int list_pop_front(List *list, void *out);

/*
 *
 *   int x;
 *   list_pop_back(list, &x);
 */
int list_pop_back(List *list, void *out);

/*
 *
 *   int x;
 *   list_get(list, 2, &x);
 */
int list_get(const List *list, size_t index, void *out);

/*
 *
 *   int x = 42;
 *   list_set(list, 1, &x);
 */
int list_set(List *list, size_t index, const void *value);

/*
 *
 *   list_remove_at(list, 0);
 */
int list_remove_at(List *list, size_t index);

/*
 *
 *   list_clear(list);
 */
void list_clear(List *list);

/*
 *
 *   void print_int(void *elem) {
 *       printf("%d\n", *(int *)elem);
 *   }
 *   list_foreach(list, print_int);
 */
void list_foreach(List *list, void (*callback)(void *elem));

#endif // LIST_H
