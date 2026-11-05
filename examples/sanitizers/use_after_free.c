#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *value = malloc(sizeof(int));
    if (!value)
        return 1;

    *value = 42;
    free(value);

    printf("*value = %d\n", *value); /* use-after-free */
    return 0;
}
