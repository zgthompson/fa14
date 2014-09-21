#include <stdlib.h>
#include <string.h>

#include "quicksort.h"

void quicksort(void *base, int count, int typeSize, compare_func comp) {

    if (count < 2) return;

    void *pivot = base + ((count / 2) * typeSize);
    void *left = base;
    void *right = base + ((count - 1) * typeSize);
    void *temp = malloc(typeSize);

    while (left <= right) {
        if ( comp(left, pivot) < 0) {
            left += typeSize;
        }
        else if ( comp(right, pivot) > 0 ) {
            right -= typeSize;
        }
        else {
            memcpy(temp, left, typeSize);
            memcpy(left, right, typeSize);
            memcpy(right, temp, typeSize);
            left += typeSize;
            right -= typeSize;
        }
    }

    free(temp);

    quicksort(base, (right - base) / typeSize + 1, typeSize, comp);
    quicksort(left, ((base + count * typeSize) - left) / typeSize, typeSize, comp);
}
