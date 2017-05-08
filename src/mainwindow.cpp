#include "inc/mainwindow.h"
#include "inc/qcustomplot.h"
#include "inc/nsga.h"
#include "inc/range_dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QGroupBox>
#include <QSlider>

void MainWindow::displayParetoFront(){
    QVector<double> x, y;
    nsga->getParetoFrontCoordinates(x,y);
    xPlotData.push_back(x);
    yPlotData.push_back(y);
    plot->graph(0)->setData(x,y);
    plot->graph(0)->rescaleAxes();
    plot->replot();
}

void MainWindow::displayScene(int time){
    generationCounter->setText("Liczba generacji: " + QString::number(time));
    plot->graph(0)->setData(xPlotData[time-1],yPlotData[time-1]);
    plot->graph(0)->rescaleAxes();
    plot->replot();
}


void MainWindow::openRangeDialog(){
    RangeDialog* dialog = new RangeDialog(&solutionRange,problemSize->value(),this);
    dialog->show();
}

void MainWindow::start(){
    xPlotData.clear();
    yPlotData.clear();
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
    timeSlider->setRange(1,currentNumberOfGenerations);
    timeSlider->setValue(currentNumberOfGenerations);
    timeSlider->setEnabled(true);
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
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setDisabled(true);
    timeSlider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

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
    connect(timeSlider,SIGNAL(valueChanged(int)),this,SLOT(displayScene(int)));

    QFormLayout* paramLayout = new QFormLayout;
    paramLayout->addRow("Rozmiar populacji:",populationSize);
    paramLayout->addRow("Wymiar problemu:",problemSize);
    paramLayout->addRow("Liczba generacji:",generationBox);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    QGroupBox* paramGroupBox = new QGroupBox("Parametry");
    paramGroupBox->setLayout(paramLayout);
    //rightLayout->addLayout(paramLayout);
    rightLayout->addWidget(paramGroupBox);
    rightLayout->addWidget(rangeDialogButton);
    rightLayout->addWidget(startButton);
    rightLayout->addStretch();
    rightLayout->addWidget(generationCounter);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(new QLabel("Oś czasu"));
    timeLayout->addWidget(timeSlider);
    leftLayout->addWidget(plot);
    leftLayout->addLayout(timeLayout);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

}

MainWindow::~MainWindow()
{

}
