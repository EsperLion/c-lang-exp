#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Hash table with separate chaining
 * ================================
 *
 * A hash table stores key/value pairs and uses a hash function to jump
 * close to a pair instead of scanning every element.
 *
 * Mental model:
 *
 *   key "cat" --hash--> 12345 --% capacity--> bucket 10
 *
 *   buckets[10] -> entry("cat", value) -> entry("dog", value) -> NULL
 *   buckets[11] -> NULL
 *   buckets[12] -> entry("bird", value) -> NULL
 *
 * The bucket array is the "directory"; each bucket is the head of a linked
 * list of entries whose keys hash to that bucket. This is called separate
 * chaining because collisions are kept in separate chains.
 *
 * How the main operations should work:
 *
 *   put:
 *     1. Compute the hash of the key.
 *     2. Find the bucket: hash % capacity.
 *     3. Walk that bucket's chain. If the key is already there, overwrite
 *        its value and return true.
 *     4. Otherwise allocate a new entry, copy the key and value, link the
 *        entry into the bucket, and increment size.
 *     5. If size / capacity is above the maximum load factor, grow the
 *        bucket array (usually double it) and rehash every entry.
 *
 *   get:
 *     1. Compute the hash and find the bucket.
 *     2. Walk the chain and compare keys with strcmp.
 *     3. Copy the stored value into the caller's buffer.
 *
 *   remove:
 *     1. Find the entry in its bucket chain.
 *     2. Unlink it, free the copied key and value, and decrement size.
 *
 * Average cost is O(1 + size / capacity), which is O(1) while the load
 * factor is bounded. Worst case is O(n) if every key lands in one bucket
 * or the hash function is bad.
 *
 * Ownership and rules:
 *   - Keys are NUL-terminated strings. The table copies each key, so the
 *     caller may free or reuse its string after put() returns.
 *   - Values are copied as value_size raw bytes. If a value contains
 *     pointers, only the pointers are copied, not the data they point to.
 *   - key must not be NULL. The empty string "" is a valid key.
 *   - value must not be NULL when calling put().
 *   - The table does not call free() on user data inside a value.
 *   - Iteration order is unspecified and may change after insert/remove.
 *   - The table is not thread-safe.
 */

/*
 * Number of buckets allocated by hash_table_create().
 * Keeping it a power of two allows index = hash & (capacity - 1), which is
 * faster than hash % capacity. The implementation may use either, as long
 * as resizing keeps the chosen rule consistent.
 */
#define HASH_TABLE_INITIAL_CAPACITY 16

/*
 * Grow when size / capacity exceeds this value. 0.75 is a common compromise:
 * short chains on average, while still leaving room before the next resize.
 */
#define HASH_TABLE_MAX_LOAD_FACTOR 0.75

/* One key/value pair in a bucket chain. */
typedef struct HashEntry {
  char *key;              /* owned copy of the NUL-terminated key */
  void *value;            /* owned copy of value_size bytes */
  size_t hash;            /* cached hash, so resizing does not recompute it */
  struct HashEntry *next; /* next entry in the same bucket, or NULL */
} HashEntry;

/* The table itself. */
typedef struct HashTable {
  HashEntry **buckets; /* array of capacity bucket heads; empty bucket is NULL */
  size_t capacity;     /* number of buckets */
  size_t size;         /* number of stored key/value pairs, not buckets */
  size_t value_size;   /* bytes per value, fixed at creation */
} HashTable;

/*
 * Visitor called by hash_table_foreach() for every entry.
 *
 * key and value are owned by the table and remain valid only for the
 * duration of the callback. Do not modify the table while iterating.
 */
typedef void (*HashTableVisitor)(const char *key, const void *value,
                                 void *context);

/**
 * Create an empty hash table.
 *
 * @param value_size Size in bytes of one value. Must be greater than zero.
 * @return A new table with HASH_TABLE_INITIAL_CAPACITY empty buckets, or
 *         NULL if value_size is zero or allocation fails.
 */
HashTable *hash_table_create(size_t value_size);

/**
 * Destroy the table and free every key, value, entry, and bucket array.
 *
 * Safe to call with NULL or with *table == NULL. After this call, *table is
 * set to NULL.
 */
void hash_table_destroy(HashTable **table);

/**
 * Insert a new key or replace the value of an existing key.
 *
 * The key and value are copied. If the key already exists, its value is
 * overwritten and the size does not change. If it is new, an entry is added
 * and the table may resize to keep the load factor bounded.
 *
 * @return true on success, false if table/key/value is NULL, value_size is
 *         invalid, or allocation fails.
 */
bool hash_table_put(HashTable *table, const char *key, const void *value);

/**
 * Look up a key and copy its value into out.
 *
 * @param out Destination buffer of at least value_size bytes. Must not be
 *            NULL.
 * @return true if the key was found, false otherwise.
 */
bool hash_table_get(const HashTable *table, const char *key, void *out);

/**
 * Check whether a key exists without copying its value.
 *
 * @return true if the key is present, false otherwise.
 */
bool hash_table_contains(const HashTable *table, const char *key);

/**
 * Remove a key and its value.
 *
 * @return true if the key was found and removed, false otherwise.
 */
bool hash_table_remove(HashTable *table, const char *key);

/**
 * Remove every entry but keep the allocated bucket array and value_size.
 * After this call, size is zero.
 */
void hash_table_clear(HashTable *table);

/**
 * Return the number of stored key/value pairs.
 */
size_t hash_table_size(const HashTable *table);

/**
 * Return the current number of buckets.
 *
 * Useful for observing load factor: hash_table_size(table) /
 * hash_table_capacity(table).
 */
size_t hash_table_capacity(const HashTable *table);

/**
 * Check whether the table contains no entries.
 *
 * @return true if the table has no entries, false if it has entries or is
 *         NULL.
 */
bool hash_table_is_empty(const HashTable *table);

/**
 * Call visit(key, value, context) once for every entry.
 *
 * The order is unspecified. If table or visit is NULL, this is a no-op.
 * The callback must not insert, remove, or clear entries during iteration.
 */
void hash_table_foreach(const HashTable *table, HashTableVisitor visit,
                        void *context);

#endif /* HASH_TABLE_H */
