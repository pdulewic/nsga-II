#include "inc/nsga.h"
#include "inc/constants.h"
#include <QDebug>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>

using namespace std;

double NSGA::objectiveFunction1(const Solution &s){
    // tutaj trzeba zaimplementować możliwość dowolnej funkcji z parsera
    return pow(s.val[0],2.0);
}

double NSGA::objectiveFunction2(const Solution &s){
    // tutaj trzeba zaimplementować możliwość dowolnej funkcji z parsera
    return pow(s.val[0]-2.0,2.0);
}

void NSGA::clearFronts(){
    for(auto x:fronts)
        x.clear();
    fronts.clear();
}

Solution NSGA::crossoverAndMutate(const Solution &dominantParent, const Solution &recesiveParent){
    Solution child(problemSize);
    for(int i=0; i<problemSize; ++i){
        child.val[i] = DOMINANT_PARENT_ADVANTAGE*dominantParent.val[i] +
                (1.0 - DOMINANT_PARENT_ADVANTAGE)*recesiveParent.val[i];
    }
    random_device r;
    default_random_engine e(r());
    uniform_real_distribution<double> dist(0.0, 1.0);
    if(dist(e) < MUTATION_PROBABILITY){
        uniform_real_distribution<double> strength(-MUTATION_STRENGTH, MUTATION_STRENGTH);
        // mutacja przebiega dla każdej składowej wektora x, chociaż dla niektórych może być bardzo mała
        for(int i=0; i<problemSize; ++i)
            child.val[i] += child.val[i]*strength(e);
    }
    child.objValue1 = objectiveFunction1(child);
    child.objValue2 = objectiveFunction2(child);
    return child;
}

void NSGA::generateRandomPopulation(const array<pair<double, double>, MAX_PROBLEM_SIZE> &range){
    population.clear();
    populationSize = givenPopulationSize;  //ustalenie parametru na czas całej symulacji
    problemSize = givenProblemSize;

    random_device r;
    default_random_engine e(r());
    vector<uniform_real_distribution<double>> dist;

    for(int i=0; i<problemSize; ++i)
        dist.push_back(uniform_real_distribution<double>(range[i].first,range[i].second));

    for(int i=0; i<populationSize; ++i){
        Solution s(problemSize);
        for(int j=0; j<problemSize; ++j)
            s.val[j] = dist[j](e);   // przydział losowych wartości z ustawionych przedziałów
        s.objValue1 = objectiveFunction1(s);
        s.objValue2 = objectiveFunction2(s);
        population.push_back(s);
    }

}

void NSGA::getParetoFrontCoordinates(QVector<double> &f1, QVector<double> &f2){
    for(const auto &x : fronts[0]){   //zamienić tutaj potem populację na pareto front
        f1.push_back(population[x].objValue1);
        f2.push_back(population[x].objValue2);
    }
}

void NSGA::fastNondominatedSort(){
    clearFronts();                    //powstaną nowe fronty pareto
    int currentPopulationSize = population.size();
    fronts.push_back(vector<int>(0)); //pierwszy front
    vector<int> n(currentPopulationSize);    //licznik elementów dominujących każde rozwiązanie
    fill(n.begin(),n.end(),0);
    vector<vector<int>> S(currentPopulationSize); //indeksy elementów dominowanych przez każde rozwiązanie
    for(int p=0; p<currentPopulationSize; ++p){
        for(int q=0; q<currentPopulationSize; ++q){
            if(population[p].dominates(population[q]))
                S[p].push_back(q);
            else if(population[q].dominates(population[p]))
                n[p]++;

        }
        if(n[p] == 0){
            population[p].nondominationRank = 1;
            fronts[0].push_back(p);
        }
    }
    int i = 0;
    while(fronts[i].size()){
        fronts.push_back(vector<int>(0));
        for(auto p: fronts[i]){
            for(auto q: S[p]){
                n[q]--;
                if(n[q] == 0){
                    population[q].nondominationRank = i+2;
                    fronts[i+1].push_back(q);
                }
            }
        }
        i++;
    }
}

void NSGA::createOffspring(){
    vector<int> mates(populationSize), mates2(populationSize);
    for(int i=0; i<populationSize; ++i)
        mates[i] = mates2[i] = i;
    random_shuffle(mates.begin(), mates.end());
    random_shuffle(mates2.begin(), mates2.end());
    mates.insert(mates.end(),make_move_iterator(mates2.begin()),make_move_iterator(mates2.end()));

    vector<Solution> offspring;

    int matesSize = mates.size();
    for(int i=0; i<matesSize; i+=4){
        if(i+3 >= matesSize)
            break;
        int parent1 = population[mates[i]] > population[mates[i+1]] ? mates[i] : mates[i+1];
        int parent2 = population[mates[i+2]] > population[mates[i+3]] ? mates[i+2] : mates[i+3];
        // każda para ma 2 dzieci, każde ma innego rodzica dominującego
        offspring.push_back(crossoverAndMutate(population[parent1],population[parent2]));
        offspring.push_back(crossoverAndMutate(population[parent2],population[parent1]));
    }
    // dołączenie potomstwa do populacji
    population.insert(population.end(),make_move_iterator(offspring.begin()),make_move_iterator(offspring.end()));
}

NSGA::NSGA(QObject *parent = nullptr): QObject(parent){
    givenPopulationSize = DEFAULT_POPULATION_SIZE;
    givenProblemSize = MIN_PROBLEM_SIZE;
}

