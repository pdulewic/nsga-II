#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"
#include <array>
#include <vector>
#include <utility>
#include <QVector>

class QCustomPlot;
class QSpinBox;
class QPushButton;
class NSGA;
class QLabel;
class QSlider;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    NSGA* nsga;
    std::array<std::pair<double,double>, MAX_PROBLEM_SIZE> solutionRange;  // zakres dozwolonych wartości zmiennych rozwiązania x
    // wektory klasy QVector są danymi wejściowymi funkcji setData klasy QCPGraph, stąd ich wymieszanie z std::vector
    std::vector<QVector<double>> xPlotData;
    std::vector<QVector<double>> yPlotData;
    int numberOfGenerations;

    QCustomPlot* plot;
    QSpinBox* populationSize;
    QSpinBox* problemSize;
    QSpinBox* generationBox;
    QPushButton* startButton;
    QPushButton* rangeDialogButton;
    QLabel* generationCounter;
    QSlider* timeSlider;
    QLineEdit* expression1;
    QLineEdit* expression2;

    void displayParetoFront();
private slots:
    void setNumberOfGenerations(int val){numberOfGenerations = val; }
    void displayScene(int time);
    void openRangeDialog();
    void start();

protected:
    QSize sizeHint() const {return QSize(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT); }

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
