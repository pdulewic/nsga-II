#ifndef NSGA_H
#define NSGA_H

#include <QObject>
#include <QVector>
#include <vector>
#include <array>
#include "solution.h"
#include "constants.h"

namespace exprtk{
    template <typename T>
    class expression;
}

class NSGA: public QObject{
    Q_OBJECT

    std::vector<Solution> population;
    std::vector<std::vector<int>> fronts;
    int givenPopulationSize;  //parametr zadany przez użytkownika, może się zmieniać w trakcie symulacji
    int populationSize;
    int givenProblemSize;
    int problemSize;


    void evaluateObjectiveFunctions(Solution& s);
    std::vector<double> arguments;
    exprtk::expression<double>* expression1;
    exprtk::expression<double>* expression2;

    void crowdingDistanceAssignment(const std::vector<int>& frontIndex);
    Solution crossoverAndMutate(const Solution& dominantParent, const Solution& recesiveParent);
public:
    NSGA(QObject* parent);
    void generateRandomPopulation(const std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>& range);
    void getParetoFrontCoordinates(QVector<double>& f1, QVector<double>& f2);
    void fastNondominatedSort();
    void createOffspring();
    void cutUnfitHalf();
    void initializeObjectiveFunctions(std::string exp1, std::string exp2);

public slots:
    void setPopulationSize(int sz){givenPopulationSize = sz;}
    void setProblemSize(int sz){givenProblemSize = sz;}

};

#endif // NSGA_H
