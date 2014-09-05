#include <stdio.h>

int binary_search(int array[], int key, int low, int high) {
    int middle;

    if (low > high) return low;

    middle = (low + high) / 2;

    //if (array[middle] == key) return middle;
    if (array[middle] > key) {
        return binary_search(array, key, low, middle - 1);
    }
    else {
        return binary_search(array, key, middle + 1, high);
    }
}

int main() {
    int array[8] = { 2, 2, 2, 4, 5, 5, 5, 6 };
    printf("%d\n", binary_search(array, 2, 0, 7));
}
