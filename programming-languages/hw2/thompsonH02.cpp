// Time spent on solution: 2-2.5 hours

#include "fraction.h"

#include <iostream>     // cin, cout
#include <vector>       // vector
#include <algorithm>    // sort



int main() {

    // optimizations that make cin and cout faster
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // preallocate plenty of space
    std::vector<Fraction> fractions;
    fractions.reserve(50000);

    // create all the fractions
    Fraction fractionSum;
    Fraction fraction;
    while (std::cin >> fraction) {
        fractions.push_back(fraction); 
        fractionSum += fraction;
    }

    std::cout << "The sum of the fractions is: " << fractionSum << "\n";

    // sort the fractions
    std::sort(fractions.begin(), fractions.end());

    // print the fractions
    for (int i = 0; i < fractions.size(); ++i) {
        std::cout << fractions[i] << "\n";
    }
}
