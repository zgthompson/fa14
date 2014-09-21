#include <stdbool.h>
#include <stdio.h>

#include "fraction.h"

bool fraction_init(FILE *fp, fraction *f) {

    int ch;
    int wholeNumber, numerator, denominator, multiplier;

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

    // deal with improper fractions
    if (numerator >= denominator) {
        wholeNumber = wholeNumber + numerator / denominator;
        numerator = numerator % denominator;
    }

    // reduce the numerator and denominator to lowest terms
    int divisor = fraction_gcd(denominator, numerator);
    numerator = numerator / divisor;
    denominator = denominator / divisor;

    // assign a value for the comparison function
    f->value = (wholeNumber + (double) numerator / denominator) * multiplier;

    // restore negative sign according to output format
    if (multiplier == -1) {
        if (wholeNumber > 0) wholeNumber = -wholeNumber;
        else numerator = -numerator;
    }

    // set remaining member variables
    f->wholeNumber = wholeNumber;
    f->numerator = numerator;
    f->denominator = denominator;

    return true;

}


int fraction_gcd(int a, int b) {

    while (b != 0) {
        int oldA = a;
        a = b;
        b = oldA % b;
    }

    return a;
}


int fraction_compare(const void *f1, const void *f2) {
    return ((fraction*)f1)->value - ((fraction*)f2)->value;
}

