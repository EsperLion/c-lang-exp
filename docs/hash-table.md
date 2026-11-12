# Hash table notes: hash functions and collision handling

These notes accompany `include/hash-table.h` and `src/hash-table.c`. They
describe the ideas and trade-offs without giving an implementation.

## 1. What a hash table does

A hash table maps a key to a value in roughly constant time:

1. Turn the key into an integer with a **hash function**.
2. Reduce that integer to an index in a fixed-size array of **buckets**.
3. Store the entry in that bucket, or look it up there.

Two different keys can produce the same bucket index. That is a
**collision**, and every hash table needs a strategy for it.

A hash table only works if two operations are defined:

- **Hashing**: key -> an integer.
- **Equality**: are these two keys the same? For strings this is usually
  `strcmp`.

The hash function decides where to look; equality decides whether the
entry found there is actually the right one.

Useful terms:

- **Capacity (m)**: number of buckets.
- **Size (n)**: number of stored key/value pairs.
- **Load factor (alpha)**: n / m. It measures how full the table is.
- **Collision**: two different keys map to the same bucket.
- **Rehashing**: rebuilding the table with a different capacity.
- **Avalanche**: a small change in the key should change about half the
  hash bits.

## 2. Properties of a good hash function

A hash function used in a table should be:

- **Deterministic**: the same key always gives the same hash.
- **Fast**: it runs on every lookup, so it should be cheap relative to the
  work it saves.
- **Uniform**: hashes should be spread evenly over the bucket indices.
  Even a perfect hash function collides sometimes (the birthday paradox),
  but a good one keeps collisions rare.
- **Avalanching**: keys that differ by one byte should not produce hashes
  that differ by only one bit or one bucket.
- **Stable within a run**: it must not change while the table is in use.

A hash function for a table does **not** need to be cryptographic. It only
needs to distribute well and be fast. Cryptography becomes relevant when
keys are controlled by an attacker (see section 7).

### Reducing a hash to a bucket index

There are two common ways:

- **Modulo**: `index = hash % capacity`. Works with any capacity. Prime
  capacities are often used because they mix the hash bits better.
- **Masking**: if capacity is a power of two,
  `index = hash & (capacity - 1)`. Faster, but it only uses the low bits
  of the hash, so the hash function must mix those bits well.

Whichever rule you choose, keep it consistent. `resize` must use the same
rule for the new capacity.

## 3. Hash functions worth trying

These are ordered roughly from easiest to most advanced.

### djb2

A classic string hash by Daniel J. Bernstein. It starts with a large
constant (5381) and, for each byte, multiplies the running value by 33 and
adds the byte. It is tiny, fast, and surprisingly usable for short ASCII
keys. Its distribution is not great for keys that differ in structured
ways, and its low bits are weak, so it is a better fit for chaining with
modulo than for power-of-two masking.

### sdbm

Similar in spirit to djb2, but multiplies by 65599. It was used by the sdbm
database library. It is simple and reasonably fast, with decent spread on
short strings, but it is not as well mixed as modern hashes.

### FNV-1a (Fowler-Noll-Vo)

A very common general-purpose choice. It starts from a fixed offset basis,
then for each byte XORs the byte into the hash and multiplies by a prime
(16777619 for 32-bit, 1099511628211 for 64-bit). It is short, fast, and
distributes well for typical keys. It is a good default for a first
implementation. The 64-bit variant is preferable on 64-bit machines.

### Jenkins one-at-a-time

A byte-at-a-time hash by Bob Jenkins. It adds the byte, performs a few
shift/add operations, and finishes with a final mixing step. It has much
better avalanche than djb2 and is still compact. A good choice if you want
to see why final mixing matters.

### MurmurHash3

A fast, non-cryptographic family designed for hash tables. It processes
data in blocks, uses multiplication and rotation to mix bits, and accepts
a seed. It has good distribution and speed, and seeded variants make
collision attacks harder. The 32-bit x86 variant is easy to study; the
128-bit variant is more complex.

### xxHash

A very fast non-cryptographic hash family (XXH32, XXH64, XXH3). It is
designed for high throughput on large inputs and still has good
distribution. It is more complex than FNV-1a, so it is a later experiment
rather than a first implementation.

### CRC32

A checksum based on polynomial division. It is excellent at detecting
accidental errors and is often hardware-accelerated. It is **not** designed
as a table hash: it is not keyed, not collision-resistant, and its
distribution is not ideal. It can work for non-adversarial data, but it is
usually the wrong tool for a general-purpose table.

### SipHash

A keyed pseudorandom function designed specifically to stop **hash
flooding** attacks. The table keeps a secret key; without it, an attacker
cannot reliably construct keys that collide. It is slower than FNV or
Murmur but still practical. Use it when keys come from untrusted input
(network requests, user-supplied query parameters, and so on).

### CityHash and FarmHash

Fast, high-quality non-cryptographic hashes from Google. They are good for
large keys and performance-sensitive workloads, but the implementations
are larger and may change between versions. They are better as a
reference than as a first hand-written hash.

### Java-style polynomial hash

The familiar `h = 31 * h + byte` used by `String.hashCode()`. It is easy to
understand and fine for a classroom example, but it has poor avalanche and
is easy to collide on purpose. Do not use it for production tables.

### Rolling hash (special case)

A polynomial hash can be updated cheaply when a window slides over a
string. That is useful for substring search, not for a general key/value
table, where keys are hashed independently.

### Randomized / universal hashing (strategy, not one function)

Choose a random seed when the table is created and mix it into the hash.
Two runs of the program then produce different bucket assignments, so an
attacker cannot precompute collisions. This can be combined with MurmurHash
or SipHash.

### Comparison

| Hash | Speed | Distribution | Keyed / attack-resistant | Best use |
| --- | --- | --- | --- | --- |
| djb2 | very fast | fair | no | small learning projects |
| sdbm | very fast | fair | no | simple string keys |
| FNV-1a | fast | good | no | default for a first table |
| Jenkins one-at-a-time | fast | very good | no | short keys, good avalanche |
| MurmurHash3 | very fast | very good | seeded variant helps | general-purpose tables |
| xxHash | extremely fast | very good | no (seed helps) | large keys, throughput |
| CRC32 | very fast (HW) | fair | no | error detection, not tables |
| SipHash | moderate | very good | yes | untrusted keys, DoS resistance |
| CityHash / FarmHash | extremely fast | very good | no | performance-critical hashing |
| Java 31x | very fast | poor | no | teaching only |

## 4. Collision resolution

There are two main families.

### 4.1 Separate chaining

Every bucket holds a collection of entries, usually a linked list. A
collision simply adds another node to the same bucket.

What the scaffold uses:

- `buckets` is an array of `HashEntry *`.
- Each `HashEntry` has `key`, `value`, `hash`, and `next`.
- Lookup walks the chain and compares keys with `strcmp`.
- Insert can add to the front (fast) or the end (stable order). Front
  insertion is simpler and makes recently inserted keys cheap to find.
- Delete unlinks a node and frees it.
- Load factor can exceed 1; the average chain length is approximately the
  load factor.

Advantages:

- Simple to implement and reason about.
- Deletion is straightforward; no tombstones are needed.
- Tolerates a high load factor better than open addressing.
- The bucket array can use any capacity, including a power of two.

Disadvantages:

- Each entry is a separate allocation, so insert/remove allocate and free.
- Following `next` pointers causes cache misses.
- Extra memory per entry for the pointer (and possibly the cached hash).
- A bad hash function can make one chain very long.

Variants:

- **Per-bucket dynamic array** instead of a linked list. Better locality,
  but deletion is more expensive and each bucket needs its own allocation.
- **Tree per bucket** (as in Java 8's `HashMap`): when a chain grows past a
  threshold, convert it to a balanced tree, improving worst-case lookup to
  O(log n). This adds a lot of complexity.
- **Move-to-front**: after a successful lookup, move the entry to the front
  of its chain. Helps when some keys are much hotter than others.
- **Ordered chain**: keep each chain sorted by key. Lookup can stop early,
  but insert becomes more expensive.

### 4.2 Open addressing

All entries live in one array. On a collision, the table probes a sequence
of alternative slots until it finds an empty one or the key.

The probe sequence is the defining choice:

- **Linear probing**: try index, index + 1, index + 2, ... . Excellent
  cache locality, but collisions create long runs called **primary
  clustering**, which slow down both lookup and insert. Capacity must be
  handled carefully (often a power of two).
- **Quadratic probing**: try index + 1, index + 4, index + 9, ... (or
  index + 1, index + 3, index + 5, ...). Reduces primary clustering but
  creates **secondary clustering**: keys with the same initial bucket
  follow the same probe sequence. Not every capacity works with every
  quadratic sequence.
- **Double hashing**: the step size comes from a second hash of the key.
  Different keys that start in the same bucket take different paths, so
  clustering is much lower. More computation per probe, and the step must
  be coprime with the capacity to visit every slot.
- **Robin Hood hashing**: when inserting, compare probe distances. If the
  new entry has travelled farther than the entry currently in the slot,
  swap them and continue with the displaced entry. This equalizes probe
  lengths and makes unsuccessful lookups cheaper. Lookup can stop as soon
  as the probe distance exceeds the distance of the entry in the slot.
- **Cuckoo hashing**: each key has two (or more) candidate buckets. Insert
  may evict an existing key, which moves to its other bucket, and so on.
  Lookup is worst-case O(1) because there are only two places to check.
  Insert can cycle, requiring a rehash; load factor usually stays around
  0.5 or lower.
- **Hopscotch hashing**: each bucket has a small neighborhood (for example
  32 slots) and entries are kept within that neighborhood of their home
  bucket. A bitmap records which slots in the neighborhood are occupied,
  making lookup a short scan with good locality.
- **Swiss table / SIMD probing**: a modern design used by Abseil and
  others. Each group of slots has control bytes marking empty, deleted, or
  occupied. SIMD compares many slots at once, making probes very fast.
  Segmenting and metadata make it complex to implement correctly.

Open addressing in general:

Advantages:

- One contiguous allocation: fewer allocations and better cache behavior.
- No per-entry `next` pointer, so memory overhead is lower.
- Lookup can be very fast, especially with linear probing or SIMD.

Disadvantages:

- Load factor must stay below roughly 0.7; performance degrades sharply as
  the table fills.
- Deletion is tricky. If you simply clear a slot, a later lookup may stop
  at the hole and miss an entry that was probed past it. Use a
  **tombstone** (a `DELETED` marker) or backward-shift deletion.
- Tombstones accumulate and slow down probes, so they must be cleaned up
  during rehash.
- Resizing is mandatory and more expensive to get right.

### 4.3 Perfect hashing (static sets only)

If the key set is known in advance and never changes, a perfect hash
function can map every key to a distinct slot with no collisions. This is
useful for compilers and lookup tables, but it is not a general dynamic
table. Minimal perfect hashing goes further and uses exactly n slots.

### 4.4 Coalesced hashing (hybrid)

Coalesced hashing combines chaining and open addressing: colliding entries
are stored in free slots of the main array and linked together. It avoids
per-entry allocation while keeping deletion simpler than pure open
addressing. It is mostly of historical interest.

### Comparison

| Strategy | Lookup worst case | Cache behavior | Deletion | Load factor | Complexity |
| --- | --- | --- | --- | --- | --- |
| Separate chaining | O(n) | poor (pointer chasing) | easy | can exceed 1 | low |
| Chaining + tree buckets | O(log n) | poor | easy | can exceed 1 | high |
| Linear probing | O(n) | excellent | tombstones / shifting | keep below ~0.7 | low |
| Quadratic probing | O(n) | very good | tombstones | keep below ~0.7 | low |
| Double hashing | O(n) | good | tombstones | keep below ~0.7 | medium |
| Robin Hood | O(n) | very good | backward shift or tombstones | keep below ~0.8 | medium |
| Cuckoo | O(1) | good | relocation | ~0.5 | high |
| Hopscotch | O(neighborhood) | excellent | shifting | high | high |
| Swiss table | O(n) probes, very small constant | excellent | tombstones / metadata | ~0.875 | very high |

The worst-case bounds assume an adversarial or terrible hash function. With
a good hash and a bounded load factor, all of these are O(1) on average.

## 5. Load factor and resizing

The load factor is the main tuning knob:

- **Separate chaining**: keep it around 0.75 to 1.0. Chains stay short, and
  memory is not wasted on empty buckets. Some implementations allow it to
  grow larger before resizing.
- **Open addressing**: keep it below about 0.7. At 0.9, linear probing
  becomes very slow because long runs of occupied slots form.
- **Cuckoo hashing**: keep it around 0.5 because two candidate buckets per
  key need free space for relocations.

Resizing works like this:

1. Allocate a new bucket array, usually twice the old capacity.
2. For every entry in the old table, recompute its bucket index using the
   new capacity and link it into the new array.
3. Free the old array.
4. Update `capacity`.

Rehashing all entries is O(n), but it happens rarely enough that the
amortized cost of insertion stays O(1). If the hash is cached in each
entry, resizing only needs the modulo/mask step, not a full rehash of the
key bytes.

Prime capacities and power-of-two capacities both work. Power-of-two
capacities make index reduction a bitmask, but they expose weak low bits in
the hash. Prime capacities cost a division but mix the hash better. Pick
one and stay consistent.

## 6. Deletion and tombstones

In separate chaining, deletion is just unlinking a node. In open
addressing, an empty slot is also the signal "the key is not here", so you
cannot simply clear a slot:

- **Tombstone**: mark the slot as deleted. Lookups continue past it;
  inserts may reuse it. Too many tombstones degrade performance, so rehash
  when the count gets high.
- **Backward-shift deletion**: after removing an entry, shift later
  entries in the probe sequence back into the hole. This avoids tombstones
  but is trickier to implement correctly.
- **Rehash after deletion**: simple but expensive if done on every remove.

## 7. Security note: hash flooding

If an attacker can choose keys, they can try to find many keys that land in
the same bucket. With a predictable hash, this turns O(1) operations into
O(n) chains and can exhaust the server. Defenses:

- Use a keyed hash such as SipHash.
- Seed a fast hash (MurmurHash, xxHash) with a random value per table.
- Limit the maximum chain length or switch to tree buckets.
- Reject absurdly large keys before hashing.

This matters for web servers, parsers, and anything that hashes
user-supplied field names or query parameters.

## 8. Suggested learning path

1. Implement `hash_table_create`, `destroy`, `size`, `capacity`, and
   `is_empty` first.
2. Add a static string hash function. Start with FNV-1a 64-bit and test it
   on a few known strings.
3. Implement `put` and `get` with separate chaining and no resizing. Test
   collisions by using a deliberately tiny capacity (for example 4).
4. Add `remove`, `contains`, `clear`, and `foreach`.
5. Add load-factor tracking and `resize` at 0.75. Verify that every key is
   still reachable after resizing.
6. Swap in djb2, Jenkins, or MurmurHash and compare chain lengths and
   lookup time.
7. As a larger exercise, rewrite the table with open addressing and linear
   probing. Add tombstones, then compare behavior and performance.
8. Implement a set on top of the same table: store only keys and use a
   one-byte dummy value, or write a separate `hash_set` API.

Useful invariants to test:

- `size` equals the number of entries reachable through the buckets.
- Every key that was inserted can be found with `get`/`contains`.
- Removing a key makes it unreachable and does not break other chains.
- Resizing preserves every key and does not change lookup results.
- The load factor stays at or below the chosen maximum after insertion.
- `destroy` frees everything (run under AddressSanitizer or Valgrind).
