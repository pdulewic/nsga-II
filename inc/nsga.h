#ifndef NSGA_H
#define NSGA_H

#include <QObject>
#include <QVector>
#include <vector>
#include <array>
#include "solution.h"
#include "constants.h"

class NSGA: public QObject{
    Q_OBJECT

    std::vector<Solution> population;
    std::vector<std::vector<int>> fronts;
    double objectiveFunction1(const Solution& s);
    double objectiveFunction2(const Solution& s);
    int givenPopulationSize;  //parametr zadany przez użytkownika, może się zmieniać w trakcie symulacji
    int populationSize;
    int givenProblemSize;
    int problemSize;

    void clearFronts();
    void crowdingDistanceAssignment(const std::vector<int>& frontIndex);

    Solution crossoverAndMutate(const Solution& dominantParent, const Solution& recesiveParent);
public:
    NSGA(QObject* parent);
    void generateRandomPopulation(const std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>& range);
    void getParetoFrontCoordinates(QVector<double>& f1, QVector<double>& f2);
    void fastNondominatedSort();
    void createOffspring();
    void cutUnfitHalf();

public slots:
    void setPopulationSize(int sz){givenPopulationSize = sz;}
    void setProblemSize(int sz){givenProblemSize = sz;}

};

#endif // NSGA_H
