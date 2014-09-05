// Time spent on solution: 1-1.5 hours

#include <cstdio>
#include <iostream>     // cin, cout, endl
#include <sstream>      // sstream
#include <string>       // string
#include <vector>       // vector
#include <cstdlib>      // abs
#include <algorithm>    // sort


class Fraction {

    public:
        Fraction(std::string inputString);
        void prettyPrint() const;
        bool operator<(Fraction other) const;
    private:
        int wholeNumber;
        int numerator;
        int denominator;
        int signMultiplier;
        double value;
        void reduceToLowestTerms();
        int gcd(int a, int b) const;
};


Fraction::Fraction(std::string inputString) {

    // read input into a stream
    std::stringstream fractionStream;
    fractionStream << inputString;

    // initialize members with stream
    fractionStream >> wholeNumber;
    fractionStream >> numerator;

    // eat the division sign
    char divisionSign;
    fractionStream >> divisionSign;

    fractionStream >> denominator;

    if (wholeNumber < 0 || numerator < 0) {
        signMultiplier = -1;
        wholeNumber = std::abs(wholeNumber);
        numerator = std::abs(numerator);
    }
    else {
        signMultiplier = 1;
    }

    reduceToLowestTerms();

    // initialize a double value for the fraction for comparision purposes
    value = (wholeNumber + (double(numerator) / denominator)) * signMultiplier;
}


// print fraction in the same format it came in as
void Fraction::prettyPrint() const {

    int wholePrint;
    int numeratorPrint;

    if (wholeNumber == 0) {
        wholePrint = wholeNumber;
        numeratorPrint = numerator * signMultiplier;
    }
    else {
        wholePrint = wholeNumber * signMultiplier;
        numeratorPrint = numerator;
    }

    printf("%d %d/%d\n", wholePrint, numeratorPrint, denominator);
    //std::cout << wholePrint << " " << numeratorPrint << "/" << denominator << "\n";
}


// natural ordering of fractions based on thier value
bool Fraction::operator<(Fraction other) const {
    return value < other.value;
}


// simplify the numerator and denominator by the greatest common divisor
void Fraction::reduceToLowestTerms() {

    int improperWhole;

    if (numerator >= denominator) {

        improperWhole = numerator / denominator;
        numerator = numerator % denominator;

        wholeNumber += improperWhole;
    }

    int divisor = gcd(denominator, numerator); 
    numerator /= divisor;
    denominator /= divisor;
}


// uses Euclid's algorithm to find the greatest common divisor
int Fraction::gcd(int a, int b) const {
    if (b == 0) return a;
    else return gcd(b, a % b);
}


int main() {

    std::ios_base::sync_with_stdio(false);

    std::vector<Fraction> fractions;

    std::string line;
    while ( getline(std::cin, line) ) {
        fractions.push_back( Fraction(line) );
    }

    std::sort(fractions.begin(), fractions.end());


    for (std::vector<Fraction>::iterator fraction = fractions.begin();
            fraction != fractions.end(); ++fraction)
    {

        fraction->prettyPrint();

    }

}
