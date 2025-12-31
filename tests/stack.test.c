#include <stdio.h>
#include <assert.h>
#include "../include/stack.h"

typedef struct {
    int x;
    int y;
} Point;

void test_stack_int() {
    printf("== test_stack_int ==\n");

    Stack *s = stack_create(sizeof(int));
    assert(s != NULL);
    assert(stack_is_empty(s));

    int a = 10, b = 20, c = 30;
    stack_push(s, &a);
    stack_push(s, &b);
    stack_push(s, &c);

    assert(stack_size(s) == 3);

    int out;
    stack_peek(s, &out);
    assert(out == 30);

    stack_pop(s, &out);
    assert(out == 30);

    stack_pop(s, &out);
    assert(out == 20);

    stack_pop(s, &out);
    assert(out == 10);

    assert(stack_is_empty(s));

    // pop from empty stack
    assert(stack_pop(s, &out) == 0);

    stack_destroy(&s);
    assert(s == NULL);

    printf("OK\n\n");
}

void test_stack_struct() {
    printf("== test_stack_struct ==\n");

    Stack *s = stack_create(sizeof(Point));

    Point p1 = {1, 2};
    Point p2 = {3, 4};

    stack_push(s, &p1);
    stack_push(s, &p2);

    Point out;
    stack_pop(s, &out);

    assert(out.x == 3);
    assert(out.y == 4);

    stack_pop(s, &out);
    assert(out.x == 1);
    assert(out.y == 2);

    stack_destroy(&s);

    printf("OK\n\n");
}

int main() {
    test_stack_int();
    test_stack_struct();
    printf("All stack tests passed ✔\n");

    return 0;
}
