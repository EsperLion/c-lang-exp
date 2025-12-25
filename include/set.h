#ifndef SET_H
#define SET_H

#include <stddef.h>

typedef struct Set Set;

/* Create empty set */
Set *set_create(void);

/* Free set memory */
void set_destroy(Set *set);

/* Add value to set */
int set_add(Set *set, int value);

/* Remove value from set */
int set_remove(Set *set, int value);

/* Check if value exists */
int set_contains(const Set *set, int value);

/* Return number of elements */
size_t set_size(const Set *set);

/* Remove all elements */
void set_clear(Set *set);

#endif
