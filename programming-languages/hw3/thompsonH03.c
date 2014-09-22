#include <stdio.h>

#include "vector.h"
#include "fraction.h"
#include "quicksort.h"

const int BUFFER_SIZE = 1048576;


int main(int argc, char **argv) {

    if (argc != 2) {
        return 1;
    }

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        return 1;
    }

    vector *fractions = vector_new(sizeof(fraction));

    fraction sum;
    sum.numerator = 0;
    sum.denominator = 1;

    fraction f;
    while ( fraction_init(fp, &f) ) {
        fraction_add(&sum, &f);
        vector_push(fractions, &f);
    }

    printf("The sum of the fractions is: ");
    fraction_print(&sum);

    int totalFractions = vector_count(fractions);

    quicksort(vector_get(fractions, 0), vector_get(fractions, totalFractions - 1), sizeof(fraction), fraction_compare);

    char buffer[BUFFER_SIZE];
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

    int i = 0;
    while (i < totalFractions) {
        fraction *f = vector_get(fractions, i++);
        fraction_print(f);
    }

    return 0;
}
