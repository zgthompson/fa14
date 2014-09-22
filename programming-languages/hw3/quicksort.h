#ifndef QUICKSORT_H_
#define QUICKSORT_H_

typedef int (*compare_func)(const void*, const void*);
void quicksort(void *left, void *right, int typeSize, compare_func comp);

#endif
