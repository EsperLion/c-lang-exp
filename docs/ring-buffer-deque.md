# Ring-buffer deque

A **deque** (double-ended queue) supports adding and removing at both ends.
A **ring buffer** is one way to implement it using a single contiguous array.

The idea is simple: the array has a fixed capacity, and the logical first
element can be anywhere inside it. When an index reaches the end, it wraps
back to 0.

## Layout

Suggested internal state:

```c
struct Deque {
    unsigned char *elements; /* raw storage: capacity * elem_size bytes */
    size_t elem_size;        /* size of one element in bytes */
    size_t size;             /* number of stored elements */
    size_t capacity;         /* number of element slots allocated */
    size_t head;             /* physical index of logical element 0 */
};
```

Invariants:

- If `capacity == 0`: `elements == NULL`, `size == 0`, `head == 0`.
- If `capacity > 0`: `head < capacity` and `size <= capacity`.
- The next write position for `push_back` is
  `tail = (head + size) % capacity`.
- Logical element `i` (where `i < size`) lives at physical index
  `(head + i) % capacity`.
- `size == 0` means empty.
- `size == capacity` means full; the next push must grow first.

Example with `capacity = 8`, `head = 6`, `size = 4`:

```
physical index:  0    1    2    3    4    5    6    7
               +----+----+----+----+----+----+----+----+
elements:      | C  | D  |    |    |    |    | A  | B  |
               +----+----+----+----+----+----+----+----+
                  ^                        ^
                tail                     head

logical:          0    1    2    3
                  A    B    C    D

physical index:   6    7    0    1
```

`push_front` would write to physical index 5.
`push_back` would write to physical index 2 (`tail`).

## Method pseudocode

### create

```
create(elem_size):
    if elem_size == 0: return NULL

    d = malloc(sizeof(Deque))
    if d == NULL: return NULL

    d.elements = NULL
    d.elem_size = elem_size
    d.size = 0
    d.capacity = 0
    d.head = 0

    return d
```

Allocation is lazy: the element array is allocated on the first push.

### destroy

```
destroy(deque_ptr):
    if deque_ptr == NULL or *deque_ptr == NULL: return

    free((*deque_ptr).elements)
    free(*deque_ptr)
    *deque_ptr = NULL
```

### size / capacity / is_empty

```
size(d):
    return d == NULL ? 0 : d.size

capacity(d):
    return d == NULL ? 0 : d.capacity

is_empty(d):
    return d != NULL and d.size == 0
```

### reserve / grow

`reserve` allocates a new, larger array, copies elements in **logical order**,
then resets `head` to 0.

```
reserve(d, new_capacity):
    if d == NULL: return 0
    if new_capacity <= d.capacity: return 1

    new_elements = malloc(new_capacity * d.elem_size)
    if new_elements == NULL: return 0

    for i in 0 .. d.size-1:
        old_index = (d.head + i) % d.capacity
        memcpy(new_elements + i * d.elem_size,
               d.elements + old_index * d.elem_size,
               d.elem_size)

    free(d.elements)
    d.elements = new_elements
    d.capacity = new_capacity
    d.head = 0
    return 1
```

Important: when `d.capacity == 0`, skip the copy loop (there is no valid
`% 0`). The deque must have `size == 0` in that case.

Suggested growth helper:

```
ensure_capacity(d, needed):
    if needed <= d.capacity: return 1

    new_capacity = d.capacity == 0 ? 4 : d.capacity
    while new_capacity < needed:
        new_capacity = new_capacity * 2

    return reserve(d, new_capacity)
```

Doubling gives amortized O(1) pushes.

### push_back

```
push_back(d, value):
    if d == NULL or value == NULL: return 0
    if not ensure_capacity(d, d.size + 1): return 0

    index = (d.head + d.size) % d.capacity
    memcpy(d.elements + index * d.elem_size, value, d.elem_size)
    d.size = d.size + 1
    return 1
```

### push_front

```
push_front(d, value):
    if d == NULL or value == NULL: return 0
    if not ensure_capacity(d, d.size + 1): return 0

    if d.head == 0:
        d.head = d.capacity - 1
    else:
        d.head = d.head - 1

    memcpy(d.elements + d.head * d.elem_size, value, d.elem_size)
    d.size = d.size + 1
    return 1
```

Grow **before** moving `head`, because growing resets `head` to 0.

### pop_front

```
pop_front(d, out):
    if d == NULL or d.size == 0: return 0

    if out != NULL:
        memcpy(out, d.elements + d.head * d.elem_size, d.elem_size)

    d.head = (d.head + 1) % d.capacity
    d.size = d.size - 1
    return 1
```

### pop_back

```
pop_back(d, out):
    if d == NULL or d.size == 0: return 0

    index = (d.head + d.size - 1) % d.capacity

    if out != NULL:
        memcpy(out, d.elements + index * d.elem_size, d.elem_size)

    d.size = d.size - 1
    return 1
```

### front / back

```
front(d, out):
    if d == NULL or d.size == 0 or out == NULL: return 0

    memcpy(out, d.elements + d.head * d.elem_size, d.elem_size)
    return 1

back(d, out):
    if d == NULL or d.size == 0 or out == NULL: return 0

    index = (d.head + d.size - 1) % d.capacity
    memcpy(out, d.elements + index * d.elem_size, d.elem_size)
    return 1
```

### get / set

```
get(d, i, out):
    if d == NULL or out == NULL or i >= d.size: return 0

    index = (d.head + i) % d.capacity
    memcpy(out, d.elements + index * d.elem_size, d.elem_size)
    return 1

set(d, i, value):
    if d == NULL or value == NULL or i >= d.size: return 0

    index = (d.head + i) % d.capacity
    memcpy(d.elements + index * d.elem_size, value, d.elem_size)
    return 1
```

### clear

```
clear(d):
    if d == NULL: return

    d.size = 0
    d.head = 0
```

Capacity and allocated storage are kept for reuse.

## Complexity

| Operation | Time |
|---|---|
| push/pop at either end | O(1) amortized |
| front/back | O(1) |
| get/set by index | O(1) |
| reserve/grow | O(n) |
| clear | O(1) |

Memory is one contiguous allocation, so iteration and access are cache-friendly
compared with a linked list.

## Common bugs to avoid

- Taking `% capacity` when `capacity == 0`.
- Pushing without checking `size == capacity` first.
- Growing without copying elements in logical order (losing the wrap).
- Using `head - 1` with `size_t` when `head == 0` (it underflows).
- Forgetting to update `size` after a push or pop.
- Using `head == tail` to detect full: with a ring buffer that can also mean
  empty. Using a separate `size` avoids the ambiguity.
- Losing the old array when `reserve` fails. Allocate into a temporary first.
- Computing byte offsets without multiplying by `elem_size`.

## Test checklist

- Create/destroy; `destroy` sets the pointer to `NULL`.
- Empty deque: size 0, capacity 0, `is_empty` true; pop/front/back return 0.
- Push back several values; verify order with `get` and `pop`.
- Push front several values; verify reverse order.
- Interleave pushes and pops at both ends.
- Force wrap-around: with capacity 4, move `head` near the end, then push
  across the boundary.
- Grow while wrapped: fill the deque, pop from the front, push more to trigger
  a resize, then verify logical order.
- `get`/`set` at index 0, `size - 1`, and `size` (must fail).
- Store a struct, not just `int`, to verify `elem_size` handling.
- `clear` keeps capacity and allows reuse.
- Run the tests with AddressSanitizer and UndefinedBehaviorSanitizer.

## Suggested implementation order

1. `create`, `destroy`, `size`, `capacity`, `is_empty`, `clear`.
2. `reserve` and the growth helper.
3. `push_back`, `pop_back`, `back`.
4. `push_front`, `pop_front`, `front`.
5. `get`, `set`.
6. Write tests after each group, especially the wrap-around cases.
