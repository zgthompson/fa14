#ifndef FRACTION_H_
#define FRACTION_H_

#include <stdbool.h>

typedef struct {
    int wholeNumber;
    int numerator;
    int denominator;
    double value;
} fraction;


bool fraction_init(fraction *f);
int fraction_gcd(int, int);
int fraction_compare(const void *f1, const void *f2);

#endif
