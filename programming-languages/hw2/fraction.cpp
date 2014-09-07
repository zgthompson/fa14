#include "fraction.h"

#include <iostream>
#include <cstdlib>


std::ostream&  operator<<(std::ostream &output, const Fraction f) {

    int wholePrint = f.numerator / f.denominator;
    int numeratorPrint = f.numerator % f.denominator;

    if (wholePrint < 0 && numeratorPrint < 0) {
        numeratorPrint = -numeratorPrint;
    }

    output << wholePrint << " " << numeratorPrint << "/" << f.denominator;
    return output;
}


std::istream& operator>>(std::istream &input, Fraction &f) {

    int wholeNumber, numerator, denominator;
    char divisionSign;
    input >> wholeNumber >> numerator >> divisionSign >> denominator;

    bool isNegative = false;

    if (wholeNumber < 0) {
        isNegative = true;
        wholeNumber = -wholeNumber;
    }
    else if (numerator < 0) {
        isNegative = true;
        numerator = -numerator;
    }

    if (!input.fail()) {
        f.numerator = numerator;
        f.denominator = denominator;

        f.reduce();

        f.numerator += wholeNumber * f.denominator;

        if (isNegative) f.numerator = -f.numerator;
    }

    return input;
}


Fraction operator+(Fraction lhs, const Fraction &rhs) {
    lhs += rhs;
    return lhs;
}


Fraction::Fraction() {
    numerator = 0;
    denominator = 1;
}


// natural ordering of fractions based on thier value
bool Fraction::operator<(const Fraction &other) const {
    return numerator * other.denominator < other.numerator * denominator;
}


Fraction& Fraction::operator+=(const Fraction &rhs) {
    numerator = (numerator * rhs.denominator) + (rhs.numerator * denominator);
    denominator *= rhs.denominator;
    reduce();
    return *this;
}


// simplify the numerator and denominator by the greatest common divisor
void Fraction::reduce() {
    int divisor = gcd(denominator, std::abs(numerator)); 
    numerator /= divisor;
    denominator /= divisor;
}


// uses Euclid's algorithm to find the greatest common divisor
int Fraction::gcd(long long a, long long b) const {

    while (b != 0) {
        long long oldA = a;
        a = b;
        b = oldA % b;
    }

    return a;
}

