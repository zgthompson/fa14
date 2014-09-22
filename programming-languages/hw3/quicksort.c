#include <stdlib.h>
#include <string.h>

#include "quicksort.h"


void quicksort(void *left, void *right, int typeSize, compare_func comp) {

    void *middle = left + (((right - left) / typeSize) / 2) * typeSize;
    void *pivot = malloc(typeSize);
    memcpy(pivot, middle, typeSize);

    void *i = left;
    void *j = right;

    void *temp = malloc(typeSize);

    while (i <= j) {
        while (comp(i, pivot) < 0) i += typeSize;
        while (comp(j, pivot) > 0) j -= typeSize;

        if (i <= j) {
            memcpy(temp, i, typeSize);
            memcpy(i, j, typeSize);
            memcpy(j, temp, typeSize);

            i += typeSize;
            j -= typeSize;
        }
    }

    free(temp);
    free(pivot);

    if (left < j) {
        quicksort(left, j, typeSize, comp);
    }

    if (i < right) {
        quicksort(i, right, typeSize, comp); 
    }

}
