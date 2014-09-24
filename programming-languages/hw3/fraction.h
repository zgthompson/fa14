#ifndef FRACTION_H_
#define FRACTION_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    long long numerator;
    long long denominator;
} fraction;

bool fraction_init(FILE *fp, fraction *f);
void fraction_add(fraction *f1, const fraction *f2);
void fraction_print(const fraction *f);
long long fraction_gcd(long long a, long long b);
int fraction_compare(const void *lhs, const void *rhs);


#endif
