#include <stdio.h>

void merge(int array[], int low, int middle, int high) {

    int buffer[high - low + 1];

    int i = 0;
    int lowPos = low;
    int highPos = middle + 1;

    while (lowPos <= middle && highPos <= high) {
        if (array[lowPos] < array[highPos]) {
            buffer[i++] = array[lowPos++];
        }
        else {
            buffer[i++] = array[highPos++];
        }
    }

    while (lowPos <= middle) buffer[i++] = array[lowPos++];
    while (highPos <= high) buffer[i++] = array[highPos++];

    int index = low;
    while (index <= high) {
        array[index] = buffer[index - low];
        ++index;
    }
}


void mergesort(int array[], int low, int high) {
    int middle;

    if (low >= high) return;

    middle = (low + high) / 2;

    mergesort(array, low, middle);
    mergesort(array, middle + 1, high);
    merge(array, low, middle, high);
}


int main() {
    int array[10] = { 123, 542, 12, 76, 32, 211, 356, 864, 1, 721 };
    
    mergesort(array, 0, 9);

    int i = 0;
    while (i < 10) {
        printf("%d ", array[i++]);
    }
    printf("\n");
}



