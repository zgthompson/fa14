#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int BUFFER_MAX = 1048576;
const int ESTIMATED_FRACTION_LENGTH = 20;


typedef struct {
    int wholeNumber;
    int numerator;
    int denominator;
    double value;
} fraction;


bool fraction_init(fraction *f);
int fraction_gcd(int, int);


bool fraction_init(fraction *f) {

    int ch;
    int wholeNumber, numerator, denominator, multiplier;

    if ((ch = getchar()) == EOF) return false;

    wholeNumber = numerator = denominator = 0;
    multiplier = 1;

    // parse wholeNumber
    if (ch == '-') multiplier = -1;
    else wholeNumber = ch - '0';
    while ( (ch = getchar()) != ' ' ) {
        wholeNumber = wholeNumber * 10 + ch - '0';
    }
    // parse numerator
    ch = getchar();
    if (ch == '-') multiplier = -1;
    else numerator = ch - '0';
    while ( (ch = getchar()) != '/' ) {
        numerator = numerator * 10 + ch - '0';
    }
    // parse denominator
    while ( (ch = getchar()) != '\n' ) {
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
    double diff = ((fraction*)f1)->value - ((fraction*)f2)->value;
    if (diff < 0) return -1;
    else return 1;
}


int main(void) {


    int arraySize = 1024;
    fraction *fractions = malloc( sizeof(fraction) * arraySize );

    int totalFractions = 0;

    fraction f;
    while ( fraction_init(&f) ) {

        fractions[totalFractions++] = f;

        if (totalFractions == arraySize) {
            arraySize = arraySize * 2;
            fractions = realloc( fractions, sizeof(fraction) * arraySize );
        }
    }

    qsort(fractions, totalFractions, sizeof(fraction), fraction_compare);

    int estimatedSpace = totalFractions * ESTIMATED_FRACTION_LENGTH;
    int bufferSize = estimatedSpace < BUFFER_MAX ? estimatedSpace : BUFFER_MAX;
    char buffer[bufferSize];
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

    int i = 0;
    while (i < totalFractions) {
        fraction f = fractions[i++];

        printf("%d %d/%d\n", f.wholeNumber, f.numerator, f.denominator);
    }

    return 0;
}
