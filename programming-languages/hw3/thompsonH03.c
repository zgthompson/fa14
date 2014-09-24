// time spent - 6 hours

#include <stdio.h>

#include "vector.h"
#include "fraction.h"
#include "quicksort.h"
#include "SetLimits.h"


int main(int argc, char **argv) {

    SetLimits();

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
    fraction *fractionArray = fractions->data;

    fraction_quicksort(fractionArray, 0, totalFractions - 1);


    int i = 0;
    while (i < totalFractions) {
        fraction_print(&fractionArray[i++]);
    }

    return 0;
}
