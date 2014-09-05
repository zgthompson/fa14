// Time spent on solution: 2-2.5 hours

#include <iostream>     // cin, cout
#include <sstream>      // osstream
#include <vector>       // vector
#include <algorithm>    // sort


class Fraction {

    public:
        Fraction(int inWholeNumber, int inNumerator, int inDenominator);
        void appendPrettyPrintToStream(std::ostringstream &stream) const;
        bool operator<(Fraction other) const;
    private:
        int wholeNumber;
        int numerator;
        int denominator;
        double value;
        int signMultiplier;
        void reduceToLowestTerms();
        int gcd(int a, int b) const;
};


Fraction::Fraction(int inWholeNumber, int inNumerator, int inDenominator) {

    // initialize members
    wholeNumber = inWholeNumber;
    numerator = inNumerator;
    denominator = inDenominator;

    // wholeNumber and numerator are magnitudes, signMultiplier holds sign information
    if (wholeNumber < 0) {
        signMultiplier = -1;
        wholeNumber = -wholeNumber;
    }
    else if (numerator < 0) {
        signMultiplier = -1;
        numerator = -numerator;
    }
    else {
        signMultiplier = 1;
    }

    reduceToLowestTerms();

    // initialize a double value for the fraction for comparision purposes
    value = (wholeNumber + (double(numerator) / denominator)) * signMultiplier;

}
// print fraction in the same format it came in as
void Fraction::appendPrettyPrintToStream(std::ostringstream &stream) const {

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

    stream << wholePrint << " " << numeratorPrint << "/" << denominator << "\n";
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

    while (b != 0) {
        int oldA = a;
        a = b;
        b = oldA % b;
    }

    return a;
}


int main() {

    // optimizations that make cin and cout faster
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // preallocate plenty of space
    std::vector<Fraction> fractions;
    fractions.reserve(50000);

    // create all the fractions
    int wholeNumber, numerator, denominator;
    char divisionSign;
    while (std::cin >> wholeNumber >> numerator >> divisionSign >> denominator) {
        fractions.push_back( Fraction(wholeNumber, numerator, denominator) );
    }

    // sort the fractions
    std::sort(fractions.begin(), fractions.end());

    // print the fractions
    std::ostringstream outputStringStream;
    for (int i = 0; i < fractions.size(); ++i) {
        fractions[i].appendPrettyPrintToStream(outputStringStream);
    }

    std::cout << outputStringStream.str();

}
