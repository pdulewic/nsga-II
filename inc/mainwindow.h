#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"
#include <array>
#include <utility>

class QCustomPlot;
class QSpinBox;
class QPushButton;
class NSGA;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    NSGA* nsga;
    std::array<std::pair<double,double>, MAX_PROBLEM_SIZE> solutionRange;  // zakres dozwolonych wartości zmiennych rozwiązania x
    int numberOfGenerations;

    QCustomPlot* plot;
    QSpinBox* populationSize;
    QSpinBox* problemSize;
    QSpinBox* generationBox;
    QPushButton* startButton;
    QPushButton* rangeDialogButton;
    QLabel* generationCounter;

    void displayParetoFront();

private slots:
    void setNumberOfGenerations(int val){numberOfGenerations = val; }
    void openRangeDialog();
    void start();

protected:
    QSize sizeHint() const {return QSize(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT); }

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
