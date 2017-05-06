#include "inc/nsga.h"
#include "inc/constants.h"
#include <QDebug>
#include <random>
#include <cmath>
#include <algorithm>

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

void NSGA::generateRandomPopulation(const array<pair<double, double>, MAX_PROBLEM_SIZE> &range){
    population.clear();

    random_device r;
    default_random_engine e(r());
    vector<uniform_real_distribution<double>> dist;

    for(int i=0; i<problemSize; ++i)
        dist.push_back(uniform_real_distribution<double>(range[i].first,range[i].second));

    for(int i=0; i<populationSize; ++i){
        Solution s(problemSize);
        for(int j=0; j<problemSize; ++j)
            s.val[j] = dist[j](e);
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
    fronts.push_back(vector<int>(0)); //pierwszy front
    vector<int> n(populationSize);    //licznik elementów dominujących każde rozwiązanie
    fill(n.begin(),n.end(),0);
    vector<vector<int>> S(populationSize); //indeksy elementów dominowanych przez każde rozwiązanie
    for(int p=0; p<populationSize; ++p){
        for(int q=0; q<populationSize; ++q){
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
    vector<int> a1(populationSize), a2(populationSize);
    for(int i=0; i<populationSize; ++i)
        a1[i] = a2[i] = i;
    random_shuffle(a1.begin(), a1.end());
    random_shuffle(a2.begin(), a2.end());

    vector<Solution> offspring;
    // dokonczyc tutaj

}

NSGA::NSGA(QObject *parent = nullptr): QObject(parent){
    populationSize = DEFAULT_POPULATION_SIZE;
    problemSize = MIN_PROBLEM_SIZE;
}

