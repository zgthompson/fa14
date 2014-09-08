// Time spent on solution: 2-2.5 hours

#include "fraction.h"

#include <iostream>     // cin, cout
#include <fstream>
#include <vector>       // vector
#include <algorithm>    // sort



int main(int argc, char* argv[]) {

    // check for proper number of command line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " FILENAME" << std::endl;
        return 1;
    }

    // optimizations that make cin and cout faster
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // open fraction file
    std::ifstream fractionFile(argv[1]);

    // exit if file was unable to open
    if (!fractionFile.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    // preallocate plenty of space
    std::vector<Fraction> fractions;
    fractions.reserve(50000);

    // create all the fractions
    Fraction fractionSum;
    Fraction fraction;
    while (fractionFile >> fraction) {
        fractions.push_back(fraction); 
        fractionSum += fraction;
    }
    
    fractionFile.close();

    std::cout << "The sum of the fractions is: " << fractionSum << "\n";

    // sort the fractions
    std::sort(fractions.begin(), fractions.end());

    // print the fractions
    for (int i = 0; i < fractions.size(); ++i) {
        std::cout << fractions[i] << "\n";
    }

    return 0;
}
