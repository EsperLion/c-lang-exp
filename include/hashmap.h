#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct HashMap HashMap;

/* Create hashmap with initial capacity */
HashMap *hashmap_create(size_t capacity);

/* Free hashmap and all entries */
void hashmap_destroy(HashMap *map);

/* Insert or update key-value pair */
int hashmap_put(HashMap *map, int key, int value);

/* Get value by key */
int hashmap_get(const HashMap *map, int key, int *out);

/* Remove key from map */
int hashmap_remove(HashMap *map, int key);

/* Return number of stored pairs */
size_t hashmap_size(const HashMap *map);

/* Remove all entries */
void hashmap_clear(HashMap *map);

#endif
