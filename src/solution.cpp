#include "inc/solution.h"

using namespace std;

Solution::Solution(int sz = 1): size(sz), crowdingDistance(0){
    val = vector<double>(size);
    // reszta zmiennych musi zostać zainicjalizowana przed użyciem
}

bool Solution::dominates(const Solution &s){
    return (objValue1 < s.objValue1 && objValue2 <= s.objValue2) ||
            (objValue1 <= s.objValue1 && objValue2 < s.objValue2) ;
}

bool operator<(const Solution &s1, const Solution &s2){
    if(s1.nondominationRank < s2.nondominationRank)
        return true;
    if(s1.nondominationRank > s2.nondominationRank)
        return false;
    return s1.crowdingDistance > s2.crowdingDistance;
}

bool operator>(const Solution &s1, const Solution &s2){
    if(s1.nondominationRank > s2.nondominationRank)
        return true;
    if(s1.nondominationRank < s2.nondominationRank)
        return false;
    return s1.crowdingDistance < s2.crowdingDistance;
}




