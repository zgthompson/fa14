#include <stdbool.h>
#include <stdio.h>

#include "fraction.h"

bool fraction_init(FILE *fp, fraction *f) {

    int ch;
    int wholeNumber, multiplier;
    long long numerator, denominator;

    if ((ch = fgetc(fp)) == EOF) return false;

    wholeNumber = numerator = denominator = 0;
    multiplier = 1;

    // parse wholeNumber
    if (ch == '-') multiplier = -1;
    else wholeNumber = ch - '0';
    while ( (ch = fgetc(fp)) != ' ' ) {
        wholeNumber = wholeNumber * 10 + ch - '0';
    }
    // parse numerator
    ch = fgetc(fp);
    if (ch == '-') multiplier = -1;
    else numerator = ch - '0';
    while ( (ch = fgetc(fp)) != '/' ) {
        numerator = numerator * 10 + ch - '0';
    }
    // parse denominator
    while ( (ch = fgetc(fp)) != '\n' ) {
        denominator = denominator * 10 + ch - '0';
    }


    // reduce the numerator and denominator to lowest terms
    long long divisor = fraction_gcd(denominator, numerator);
    numerator = numerator / divisor;
    denominator = denominator / divisor;

    numerator += wholeNumber * denominator;
    numerator *= multiplier;

    // set remaining member variables
    f->numerator = numerator;
    f->denominator = denominator;

    return true;

}


long long fraction_gcd(long long a, long long b) {
    while (b != 0) {
        long long oldA = a;
        a = b;
        b = oldA % b;
    }

    return a;
}


void fraction_add(fraction *f1, const fraction *f2) {
    f1->numerator = (f1->numerator * f2->denominator) + (f2->numerator * f1->denominator);
    f1->denominator *= f2->denominator;

    long long divisor = fraction_gcd(f1->denominator, f1->numerator);
    f1->numerator /= divisor;
    f1->denominator /= divisor;
}

void fraction_print(const fraction *f) {
    int wholePrint = f->numerator / f->denominator;
    int numeratorPrint = f->numerator % f->denominator;

    if (wholePrint < 0 && numeratorPrint < 0) {
        numeratorPrint = -numeratorPrint;
    }

    printf("%d %d/%d\n", wholePrint, numeratorPrint, (int)f->denominator);
}


int fraction_compare(const void *lhs, const void *rhs) {
    fraction *f1 = (fraction*)lhs;
    fraction *f2 = (fraction*)rhs;
    return (f1->numerator * f2-> denominator) - (f2->numerator * f1->denominator);
}
