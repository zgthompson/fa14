#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "fraction.h"

typedef int (*compare_func)(const void*, const void*);
void quicksort(void *left, void *right, int typeSize, compare_func comp);
void fraction_quicksort(fraction *array, int left, int right);

#endif
