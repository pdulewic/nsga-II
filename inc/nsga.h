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

// typ wyliczeniowy określający typy funkcji celu
enum class FunctionType {CUSTOM, ROSENBROCK, GEEM, ACKLEY, RASTRIGIN, GOLDSTEIN_PRICE};
// Każdy typ musi mieć swoją nazwę, rozmiar wektora functionNames traktowany jest
// jak liczba dostępnych typów.
const std::vector<QString> functionNames {"Funkcja użytkownika", "Funkcja Rosenbrock'a"};

enum class TestType {BINH_KORN, CHAKONG_HAIMES, FONSECA_FLEMING};
const std::vector<QString> testNames {"Funkcja Binh'a-Korn'a (n = 2)", "Funkcja Chakong'a-Haimes'a (n = 2)", "Funkcja Fonseca-Fleminga"};

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
    FunctionType objType1;
    FunctionType objType2;
    TestType testFunctionType;
    bool isTestFunctionActivated;

    void crowdingDistanceAssignment(const std::vector<int>& frontIndex);
    Solution crossoverAndMutate(const Solution& dominantParent, const Solution& recesiveParent);
public:
    NSGA(QObject* parent);
    void generateRandomPopulation(const std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>& range);
    void getParetoFrontCoordinates(QVector<double>& f1, QVector<double>& f2);
    std::vector<Solution> getParetoFront();
    void fastNondominatedSort();
    void createOffspring();
    void cutUnfitHalf();
    void initializeObjectiveFunctions(std::string exp1, std::string exp2);

public slots:
    void setPopulationSize(int sz){givenPopulationSize = sz;}
    void setProblemSize(int sz){givenProblemSize = sz;}
    void setObjType1(int type) {objType1 = static_cast<FunctionType>(type); }
    void setObjType2(int type) {objType2 = static_cast<FunctionType>(type); }
    void setTestFunctionType(int type) {testFunctionType = static_cast<TestType>(type); }
    void activateTestFunction(bool activate) {isTestFunctionActivated = activate; }
};

#endif // NSGA_H
