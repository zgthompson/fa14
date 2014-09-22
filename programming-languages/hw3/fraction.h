#ifndef FRACTION_H_
#define FRACTION_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    long long numerator;
    long long denominator;
    double value;
} fraction;


bool fraction_init(FILE *fp, fraction *f);
long long fraction_gcd(long long, long long);
int fraction_compare(const void *f1, const void *f2);
void fraction_add(fraction *f1, const fraction *f2);
void fraction_print(const fraction *f);

#endif
