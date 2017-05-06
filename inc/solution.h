#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

/* Klasa rozwiązania - wektor x należący do R^n
 * n reprezentowane jest przez zmienną size */
struct Solution{
    int size;
    std::vector<double> val; // wektor x
    double objValue1;        // wartość pierwszej funkcji celu dla rozwiązania x
    double objValue2;        // wartość drugiej funkcji celu dla rozwiązania x
    int nondominationRank;   // współczynnik jakości rozwiązania
    double crowdingDistance; // współczynnik odległości od reszty rozwiązań

    Solution(int sz);
    bool dominates(const Solution& s);
};

// crowded comparsion operator - wiekszy == lepszy
bool operator< (const Solution& s1, const Solution& s2);
bool operator> (const Solution& s1, const Solution& s2);

#endif // SOLUTION_H
