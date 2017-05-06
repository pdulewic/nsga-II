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
#include <QDebug>

void MainWindow::displayParetoFront(){
    QVector<double> x, y;
    nsga->getParetoFrontCoordinates(x,y);
    plot->graph(0)->setData(x,y);
    // todo: ustawić lepszy dobór zakresu plota
    plot->xAxis->setRange(-1, 100);
    plot->yAxis->setRange(-1,100);
    plot->replot();
}

void MainWindow::openRangeDialog(){
    RangeDialog* dialog = new RangeDialog(&solutionRange,problemSize->value(),this);
    dialog->show();
}

void MainWindow::start(){
    nsga->generateRandomPopulation(solutionRange);
    nsga->fastNondominatedSort();
    nsga->createOffspring();
    nsga->fastNondominatedSort();
    displayParetoFront();
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    nsga = new NSGA(this);

    populationSize = new QSpinBox;
    populationSize->setRange(MIN_POPULATION_SIZE,MAX_POPULATION_SIZE);
    populationSize->setValue(DEFAULT_POPULATION_SIZE);

    problemSize = new QSpinBox;
    problemSize->setRange(MIN_PROBLEM_SIZE,MAX_PROBLEM_SIZE );
    problemSize->setValue(MIN_PROBLEM_SIZE);

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

    connect(populationSize,SIGNAL(valueChanged(int)),nsga,SLOT(setPopulationSize(int)));
    connect(problemSize,SIGNAL(valueChanged(int)),nsga,SLOT(setProblemSize(int)));
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(start()) );
    connect(rangeDialogButton,SIGNAL(clicked(bool)),this,SLOT(openRangeDialog()));

    QFormLayout* paramLayout = new QFormLayout;
    paramLayout->addRow("Rozmiar populacji:",populationSize);
    paramLayout->addRow("Wymiar problemu:",problemSize);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addLayout(paramLayout);
    rightLayout->addWidget(rangeDialogButton);
    rightLayout->addWidget(startButton);
    rightLayout->addStretch();

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
