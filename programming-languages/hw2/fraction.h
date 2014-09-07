#ifndef FRACTION_ZGT_H
#define FRACTION_ZGT_H

#include <iostream>
#include <sstream>

class Fraction {


    public:

        friend std::ostream&  operator<<(std::ostream &output, const Fraction f);
        friend std::istream& operator>>(std::istream &input, Fraction &f);
        friend Fraction operator+(Fraction lhs, const Fraction &rhs);

        Fraction();
        bool operator<(const Fraction &other) const;
        Fraction& operator+=(const Fraction &rhs);

        long long numerator;
        long long denominator;

    private:
        void reduce();
        int gcd(long long a, long long b) const;
};

#endif
