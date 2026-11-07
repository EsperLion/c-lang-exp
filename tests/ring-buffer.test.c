#include <assert.h>
#include <stdio.h>

#include "../include/ring-buffer.h"

typedef struct {
    int x;
    int y;
} Point;

/*
 * The ring buffer is opaque and has no index/iterator API, so the only way
 * to print its contents is to pop them. These helpers drain `count`
 * elements, print them oldest-to-newest, and leave the buffer empty.
 */
static void print_int_buffer(RingBuffer *rb, size_t count) {
    int value;

    printf("Ring buffer data (oldest -> newest):\n");
    for (size_t i = 0; i < count; i++) {
        if (!ring_buffer_pop(rb, &value)) {
            printf("  <buffer empty at index %zu>\n", i);
            return;
        }
        printf("  [%zu] = %d\n", i, value);
    }
}

static void print_point_buffer(RingBuffer *rb, size_t count) {
    Point point;

    printf("Ring buffer data (oldest -> newest):\n");
    for (size_t i = 0; i < count; i++) {
        if (!ring_buffer_pop(rb, &point)) {
            printf("  <buffer empty at index %zu>\n", i);
            return;
        }
        printf("  [%zu] = {x=%d, y=%d}\n", i, point.x, point.y);
    }
}

void test_ring_buffer_int(void) {
    printf("== test_ring_buffer_int ==\n");

    RingBuffer *rb = ring_buffer_create(4, sizeof(int));
    assert(rb != NULL);

    int values[] = {10, 20, 30, 40, 50};

    assert(ring_buffer_push(rb, &values[0]));
    assert(ring_buffer_push(rb, &values[1]));
    assert(ring_buffer_push(rb, &values[2]));
    assert(ring_buffer_push(rb, &values[3]));
    assert(ring_buffer_is_full(rb));

    /*
     * Buffer is full: push overwrites the oldest value.
     * [10, 20, 30, 40] + 50 -> [20, 30, 40, 50]
     */
    assert(ring_buffer_push(rb, &values[4]));
    assert(ring_buffer_is_full(rb));

    int out = 0;
    assert(ring_buffer_pop(rb, &out));
    assert(out == 20);
    assert(ring_buffer_pop(rb, &out));
    assert(out == 30);

    /* Remaining logical order: 40, 50. */
    print_int_buffer(rb, 2);

    /* print_int_buffer drained the buffer. */
    assert(!ring_buffer_pop(rb, &out));

    ring_buffer_destroy(&rb);
    assert(rb == NULL);
    printf("OK\n\n");
}

void test_ring_buffer_struct(void) {
    printf("== test_ring_buffer_struct ==\n");

    RingBuffer *rb = ring_buffer_create(3, sizeof(Point));
    assert(rb != NULL);

    Point p1 = {1, 2};
    Point p2 = {3, 4};
    Point p3 = {5, 6};
    Point p4 = {7, 8};

    assert(ring_buffer_push(rb, &p1));
    assert(ring_buffer_push(rb, &p2));
    assert(ring_buffer_push(rb, &p3));
    assert(ring_buffer_is_full(rb));

    /*
     * Buffer is full: push overwrites the oldest value.
     * [p1, p2, p3] + p4 -> [p2, p3, p4]
     */
    assert(ring_buffer_push(rb, &p4));
    assert(ring_buffer_is_full(rb));

    Point out;
    assert(ring_buffer_pop(rb, &out));
    assert(out.x == 3);
    assert(out.y == 4);

    /* Remaining logical order: p3, p4. */
    print_point_buffer(rb, 2);

    /* print_point_buffer drained the buffer. */
    assert(!ring_buffer_pop(rb, &out));

    ring_buffer_destroy(&rb);
    assert(rb == NULL);
    printf("OK\n\n");
}

int main(void) {
    test_ring_buffer_int();
    test_ring_buffer_struct();
    printf("All ring buffer tests passed ✔\n");

    return 0;
}
