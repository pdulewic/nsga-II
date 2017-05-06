#include "inc/mainwindow.h"
#include "inc/qcustomplot.h"
#include "inc/nsga.h"
#include "inc/range_dialog.h"
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

void MainWindow::displayParetoFront(){
    QVector<double> x, y;
    nsga->getParetoFrontCoordinates(x,y);
    plot->graph(0)->setData(x,y);
    plot->graph(0)->rescaleAxes();
    plot->replot();
}

void MainWindow::openRangeDialog(){
    RangeDialog* dialog = new RangeDialog(&solutionRange,problemSize->value(),this);
    dialog->show();
}

void MainWindow::start(){
    nsga->generateRandomPopulation(solutionRange);
    nsga->fastNondominatedSort();

    int currentNumberOfGenerations = numberOfGenerations;
    for(int i=0; i<currentNumberOfGenerations; ++i){
        nsga->createOffspring();
        nsga->fastNondominatedSort();
        nsga->cutUnfitHalf();
        displayParetoFront();
        generationCounter->setText("Liczba generacji: " + QString::number(i+1));
    }
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), numberOfGenerations(DEFAULT_GENERATIONS){
    nsga = new NSGA(this);

    populationSize = new QSpinBox;
    populationSize->setRange(MIN_POPULATION_SIZE,MAX_POPULATION_SIZE);
    populationSize->setValue(DEFAULT_POPULATION_SIZE);

    problemSize = new QSpinBox;
    problemSize->setRange(MIN_PROBLEM_SIZE,MAX_PROBLEM_SIZE );
    problemSize->setValue(MIN_PROBLEM_SIZE);

    generationBox = new QSpinBox;
    generationBox->setRange(MIN_GENERATIONS, MAX_GENERATIONS);
    generationBox->setValue(DEFAULT_GENERATIONS);

    generationCounter = new QLabel("Liczba generacji: 0");

    startButton = new QPushButton("Start");
    rangeDialogButton = new QPushButton("Zakres wartości x");

    for(int i=0; i<MAX_PROBLEM_SIZE; ++i)
        solutionRange[i] = std::make_pair(-DEFAULT_X_RANGE,DEFAULT_X_RANGE);

    plot = new QCustomPlot;
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    plot->addGraph();
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,5));
    plot->xAxis->setLabel("f1");
    plot->yAxis->setLabel("f2");
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, "Zbiór Pareto w przestrzeni funkcyjnej", QFont("sans", 12, QFont::Bold)));

    connect(populationSize,SIGNAL(valueChanged(int)),nsga,SLOT(setPopulationSize(int)));
    connect(problemSize,SIGNAL(valueChanged(int)),nsga,SLOT(setProblemSize(int)));
    connect(generationBox,SIGNAL(valueChanged(int)),this,SLOT(setNumberOfGenerations(int)));
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(start()) );
    connect(rangeDialogButton,SIGNAL(clicked(bool)),this,SLOT(openRangeDialog()));

    QFormLayout* paramLayout = new QFormLayout;
    paramLayout->addRow("Rozmiar populacji:",populationSize);
    paramLayout->addRow("Wymiar problemu:",problemSize);
    paramLayout->addRow("Liczba generacji:",generationBox);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addLayout(paramLayout);
    rightLayout->addWidget(rangeDialogButton);
    rightLayout->addWidget(startButton);
    rightLayout->addStretch();
    rightLayout->addWidget(generationCounter);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(plot);
    layout->addLayout(rightLayout);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

}

MainWindow::~MainWindow()
{

}
