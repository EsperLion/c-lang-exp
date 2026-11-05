#include <stdio.h>

int main(void) {
    int value = 2147483647;
    value += 1; /* signed integer overflow: undefined behavior */

    printf("value = %d\n", value);
    return 0;
}
