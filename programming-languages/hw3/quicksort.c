#include <stdlib.h>
#include <string.h>

#include "quicksort.h"

const int SELECTION_SORT_THRESHOLD = 10;


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


void fraction_quicksort(fraction *array, int left, int right) {

    fraction pivot = array[(right + left) / 2];

    int i = left;
    int j = right;

    while (i <= j) {
        while (array[i].value < pivot.value) ++i;
        while (array[j].value > pivot.value) --j;

        if (i <= j) {
            fraction temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            ++i;
            --j;
        }
    }

    if (left < j) {
        if (j - left < SELECTION_SORT_THRESHOLD) {
            fraction_insertionsort(array, left, j);
        }
        else {
            fraction_quicksort(array, left, j);
        }
    }

    if (i < right) {
        if (right - i < SELECTION_SORT_THRESHOLD) {
            fraction_insertionsort(array, i, right);
        }
        else {
            fraction_quicksort(array, i, right);
        }
    }
}


void fraction_insertionsort(fraction *array, int left, int right) {
    int i = left + 1;
    while (i <= right) {
        fraction f = array[i];
        int j = i;
        while (j > left && array[j-1].value > f.value) {
            array[j] = array[j-1];
            --j;
        }
        array[j] = f;
        ++i;
    }
}
