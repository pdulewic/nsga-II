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
    int populationSize;
    int problemSize;

    void clearFronts();
public:
    NSGA(QObject* parent);
    void generateRandomPopulation(const std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>& range);
    void getParetoFrontCoordinates(QVector<double>& f1, QVector<double>& f2);
    void fastNondominatedSort();
    void createOffspring();

public slots:
    void setPopulationSize(int sz){populationSize = sz;}
    void setProblemSize(int sz){problemSize = sz;}

};

#endif // NSGA_H
