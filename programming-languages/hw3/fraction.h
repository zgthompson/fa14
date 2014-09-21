#ifndef FRACTION_H_
#define FRACTION_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int wholeNumber;
    int numerator;
    int denominator;
    double value;
} fraction;


bool fraction_init(FILE *fp, fraction *f);
int fraction_gcd(int, int);
int fraction_compare(const void *f1, const void *f2);

#endif
