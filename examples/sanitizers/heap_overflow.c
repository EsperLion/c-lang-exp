#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *values = malloc(2 * sizeof(int));
    if (!values)
        return 1;

    values[0] = 10;
    values[1] = 20;
    values[2] = 30; /* heap buffer overflow: only 2 ints were allocated */

    printf("values[2] = %d\n", values[2]);
    free(values);
    return 0;
}
