#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct List List;

/* Create empty list */
List *list_create(void);

/* Free list and all nodes */
void list_destroy(List *list);

/* Return number of elements */
size_t list_size(const List *list);

/* Insert element at beginning */
int list_push_front(List *list, int value);

/* Insert element at end */
int list_push_back(List *list, int value);

/* Remove first element */
int list_pop_front(List *list, int *out);

/* Remove last element */
int list_pop_back(List *list, int *out);

/* Get element by index */
int list_get(const List *list, size_t index, int *out);

/* Remove all elements */
void list_clear(List *list);

#endif
