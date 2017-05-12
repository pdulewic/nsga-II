#include "inc/nsga.h"
#include "inc/constants.h"
#include "inc/exprtk.h"
#include <QDebug>
#include <QString>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>

using namespace std;

void NSGA::evaluateObjectiveFunctions(Solution &s){
    // todo: dodać więcej gotowych, bardziej skomplikowanych funkcji do wyboru.
    for(uint i = 0; i<arguments.size(); ++i)
        arguments[i] = s.val[i];
    s.objValue1 = expression1->value();
    s.objValue2 = expression2->value();
}

void NSGA::crowdingDistanceAssignment(const std::vector<int> &frontIndex){
    vector<Solution*> front;

    for(const auto x:frontIndex){
        front.push_back(&population[x]);
        front.back()->crowdingDistance = 0;
    }

    // posortuj front Pareto od najmniejszej wartości pierwszej funkcji celu, do największej
    sort(front.begin(), front.end(), [](Solution* a,Solution* b){return a->objValue1 < b->objValue1;} );
    // skrajne wartości otrzymują bardzo duży wskaźnik crowdingDistance (ale nie maksymalną
    // wartość typu double, bo mogą jeszcze wzrosnąć w kolejnym kroku)
    front.front()->crowdingDistance = front.back()->crowdingDistance = BIG_DOUBLE;
    double deltaF = front.back()->objValue1 - front.front()->objValue1;
    for(auto i = front.begin()+1; i<front.end()-1; ++i){
        (*i)->crowdingDistance += ((*(i+1))->objValue1 - (*(i-1))->objValue1)/deltaF;
    }

    // to samo, tylko względem drugiej funkcji celu
    sort(front.begin(), front.end(), [](Solution* a,Solution* b){return a->objValue2 < b->objValue2;} );
    front.front()->crowdingDistance = front.back()->crowdingDistance = BIG_DOUBLE;
    deltaF = front.back()->objValue2 - front.front()->objValue2;
    for(auto i = front.begin()+1; i<front.end()-1; ++i){
        (*i)->crowdingDistance += ((*(i+1))->objValue2 - (*(i-1))->objValue2)/deltaF;
    }
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
    evaluateObjectiveFunctions(child);
    return child;
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
            s.val[j] = dist[j](e);   // przydział losowych wartości z ustawionych przedziałów
        evaluateObjectiveFunctions(s);
        population.push_back(s);
    }

}

void NSGA::getParetoFrontCoordinates(QVector<double> &f1, QVector<double> &f2){
    for(const auto &x : population){
        if(x.nondominationRank == 1){
            f1.push_back(x.objValue1);
            f2.push_back(x.objValue2);
        }
    }
}

std::vector<Solution> NSGA::getParetoFront(){
    std::vector<Solution> front;
    for(auto x : population){
        if(x.nondominationRank == 1)
            front.push_back(x);
    }
    return front;
}

void NSGA::fastNondominatedSort(){
    fronts.clear();                    //powstaną nowe fronty pareto
    int currentPopulationSize = population.size();   //populacja może być powiększona o potomstwo
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
        int parent1 = population[mates[i]] < population[mates[i+1]] ? mates[i] : mates[i+1];
        int parent2 = population[mates[i+2]] < population[mates[i+3]] ? mates[i+2] : mates[i+3];
        // każda para ma 2 dzieci, każde ma innego rodzica dominującego
        offspring.push_back(crossoverAndMutate(population[parent1],population[parent2]));
        offspring.push_back(crossoverAndMutate(population[parent2],population[parent1]));
    }
    // dołączenie potomstwa do populacji
    population.insert(population.end(),make_move_iterator(offspring.begin()),make_move_iterator(offspring.end()));
}

void NSGA::cutUnfitHalf(){
    int numberOfAccepted = 0; //liczba zaakceprowanych do tej pory rozwiązań
    int i=0;
    while(numberOfAccepted + static_cast<int>(fronts[i].size()) <= populationSize){
        crowdingDistanceAssignment(fronts[i]);
        numberOfAccepted += fronts[i].size();
        i++;
    }
    int freePlaces = populationSize - numberOfAccepted;
    if(freePlaces)
        crowdingDistanceAssignment(fronts[i]);
    sort(population.begin(), population.end());  // o wypadkowej jakości decyduje crowded comparsion operator
    // po powyższej operacji wektor fronts znajduje się w nieprawidłowym stanie do kolejnego
    // wywołania funkcji fastNondominatedSort
    population.erase(population.begin()+populationSize, population.end()); // survival of the fittest
}

void NSGA::initializeObjectiveFunctions(string exp1, string exp2){
    populationSize = givenPopulationSize;  //ustalenie parametru na czas całej symulacji
    problemSize = givenProblemSize;

    exprtk::parser<double> parser;
    arguments = vector<double>(problemSize);
    exprtk::symbol_table<double> symbolTable;
    for(int i = 0; i<problemSize; ++i){
        QString tmp = "x" + QString::number(i+1);
        symbolTable.add_variable(tmp.toStdString(),arguments[i]);
    }

    delete expression1;
    expression1 = new exprtk::expression<double>;
    expression1->register_symbol_table(symbolTable);
    parser.compile(exp1,*expression1);

    delete expression2;
    expression2 = new exprtk::expression<double>;
    expression2->register_symbol_table(symbolTable);
    parser.compile(exp2,*expression2);
}

NSGA::NSGA(QObject *parent = nullptr): QObject(parent),
    expression1(nullptr), expression2(nullptr){
    givenPopulationSize = DEFAULT_POPULATION_SIZE;
    givenProblemSize = MIN_PROBLEM_SIZE;
}

