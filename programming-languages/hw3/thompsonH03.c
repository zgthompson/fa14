#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "vector.h"
#include "fraction.h"

const int BUFFER_SIZE = 1048576;


int main(void) {


    //int arraySize = 1024;
    vector *fractions = vector_new(sizeof(fraction));

    fraction f;
    while ( fraction_init(&f) ) {
        vector_push(fractions, &f);
    }

    int totalFractions = vector_count(fractions);

    qsort(fractions->data, totalFractions, sizeof(fraction), fraction_compare);

    char buffer[BUFFER_SIZE];
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

    int i = 0;
    while (i < totalFractions) {

        fraction f;
        vector_copy_item(fractions, &f, i++);

        printf("%d %d/%d\n", f.wholeNumber, f.numerator, f.denominator);
    }

    return 0;
}
