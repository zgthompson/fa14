#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <string.h>

typedef struct {
    void *data;
    int typeSize;
    int capacity;
    int count;
} vector;

vector *vector_new(int typeSize);
void vector_grow(vector *v);
void *vector_get(vector *v, int index);
void vector_copy_item(vector *v, void* dest, int index);
int vector_count(vector *v);
void vector_push(vector *v, void *data);


#endif
