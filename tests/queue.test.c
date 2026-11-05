#include <stdio.h>
#include <assert.h>
#include "../include/queue.h"

typedef struct {
    int id;
    const char *name;
} Item;

void test_queue_int() {
    printf("== test_queue_int ==\n");

    Queue *q = queue_create(sizeof(int));
    assert(q != NULL);
    assert(queue_is_empty(q));

    int a = 1, b = 2, c = 3;

    queue_enqueue(q, &a);
    queue_enqueue(q, &b);
    queue_enqueue(q, &c);

    assert(queue_size(q) == 3);

    int out;

    queue_peek(q, &out);
    assert(out == 1);

    queue_dequeue(q, &out);
    assert(out == 1);

    queue_dequeue(q, &out);
    assert(out == 2);

    queue_dequeue(q, &out);
    assert(out == 3);

    assert(queue_is_empty(q));

    /* dequeue from empty queue */
    assert(queue_dequeue(q, &out) == 0);

    queue_destroy(&q);
    assert(q == NULL);

    printf("OK\n\n");
}

void test_queue_struct() {
    printf("== test_queue_struct ==\n");

    Queue *q = queue_create(sizeof(Item));
    assert(q != NULL);

    Item i1 = { .id = 1, .name = "first" };
    Item i2 = { .id = 2, .name = "second" };
    Item i3 = { .id = 3, .name = "third" };

    queue_enqueue(q, &i1);
    queue_enqueue(q, &i2);
    queue_enqueue(q, &i3);

    Item out;

    queue_dequeue(q, &out);
    assert(out.id == 1);
    assert(out.name == i1.name);

    queue_dequeue(q, &out);
    assert(out.id == 2);
    assert(out.name == i2.name);

    queue_dequeue(q, &out);
    assert(out.id == 3);
    assert(out.name == i3.name);

    queue_destroy(&q);

    printf("OK\n\n");
}

int main() {
    test_queue_int();
    test_queue_struct();

    printf("All queue tests passed ✔\n");
    return 0;
}
