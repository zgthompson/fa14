#include <stdlib.h>

#include "vector.h"

vector *vector_new(int typeSize) {
    vector *v = malloc(sizeof(vector));
    v->typeSize = typeSize;
    v->capacity = 1024;
    v->count = 0;
    v->data = malloc(v->typeSize * v->capacity);
    return v;
}


void vector_grow(vector *v) {
    v->capacity *= 2;
    v->data = realloc( v->data, v->typeSize * v->capacity);
}

void vector_push(vector *v, void *data) {

    if (v->count == v->capacity) {
        vector_grow(v);
    }

    memcpy(v->data + (v->count * v->typeSize), data, v->typeSize);
    v->count++;
}


void *vector_get(vector *v, int index) {
    return v->data + (index * v->typeSize);
}


void vector_copy_item(vector *v, void* dest, int index) {
    memcpy(dest, vector_get(v, index), v->typeSize);
}



int vector_count(vector *v) {
    return v->count;
}

