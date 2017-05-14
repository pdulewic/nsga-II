#include "inc/mainwindow.h"
#include "inc/qcustomplot.h"
#include "inc/nsga.h"
#include "inc/range_dialog.h"
#include "inc/pareto_dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QGroupBox>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QMenu>

void MainWindow::displayParetoFront(){
    QVector<double> x, y;
    nsga->getParetoFrontCoordinates(x,y);
    xPlotData.push_back(x);
    yPlotData.push_back(y);
    plot->graph(0)->setData(x,y);
    if(!fixedScale->isChecked())
        plot->graph(0)->rescaleAxes();
    plot->replot();
}

void MainWindow::createMenus(){
    QMenu* fileMenu = menuBar()->addMenu("Eksportuj");

    QAction* pdfAct = new QAction("Eksportuj jako PDF",this);
    connect(pdfAct,SIGNAL(triggered(bool)),this,SLOT(exportAsPDF()));
    fileMenu->addAction(pdfAct);

    QAction* pngAct = new QAction("Eksportuj jako PNG",this);
    connect(pngAct,SIGNAL(triggered(bool)),this,SLOT(exportAsPNG()));
    fileMenu->addAction(pngAct);

}

void MainWindow::displayScene(int time){
    generationCounter->setText("Liczba generacji: " + QString::number(time));
    plot->graph(0)->setData(xPlotData[time-1],yPlotData[time-1]);
    if(!fixedScale->isChecked())
        plot->graph(0)->rescaleAxes();
    plot->replot();
}


void MainWindow::openRangeDialog(){
    RangeDialog* dialog = new RangeDialog(&solutionRange,problemSize->value(),this);
    dialog->show();
}

void MainWindow::openTableDialog(){
    ParetoDialog* dialog = new ParetoDialog;
    dialog->setTable(nsga->getParetoFront());
    dialog->show();
}

void MainWindow::start(){
    timeSlider->setEnabled(false);
    xPlotData.clear();
    yPlotData.clear();
    if(nsga->initializeObjectiveFunctions(expression1->text().toStdString(),
                                       expression2->text().toStdString()))
        return;
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

void MainWindow::disableExpression1(int x){
    if(x == 0){
        expression1->show();
        exprLayout->labelForField(expression1)->show();
    }
    else{
        expression1->hide();
        exprLayout->labelForField(expression1)->hide();
    }
}

void MainWindow::disableExpression2(int x){
    if(x == 0){
        expression2->show();
        exprLayout->labelForField(expression2)->show();
    }
    else{
        expression2->hide();
        exprLayout->labelForField(expression2)->hide();
    }
}

void MainWindow::adjustProblemSize(){
    int type = preparedType->currentIndex();
    switch (static_cast<TestType>(type)) {
    case TestType::BINH_KORN:
    //case TestType::CHAKONG_HEIMES:
        problemSize->setValue(2);
        break;
    case TestType::KURSAWE:
        problemSize->setValue(3);
        break;
    /*case TestType::OSYCZKA_KUNDU:
        problemSize->setValue(6);
        break;*/
    default:
        break;
    }
}

void MainWindow::exportAsPDF(){
    QString filename = QFileDialog::getSaveFileName(this,"Zapisz plik jako",".","Pliki PDF (.pdf)");
    if(filename.isEmpty())
        return;
    if(!filename.endsWith(".pdf"))
        filename.append(".pdf");
    plot->savePdf(filename);
}

void MainWindow::exportAsPNG(){
    QString filename = QFileDialog::getSaveFileName(this,"Zapisz plik jako",".","Pliki PNG (.png)");
    if(filename.isEmpty())
        return;
    if(!filename.endsWith(".png"))
        filename.append(".png");
    plot->savePng(filename);
}


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), numberOfGenerations(DEFAULT_GENERATIONS){
    setWindowTitle("Algorytm NSGA-II dla zadania dwukryterialnego");
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
    paretoDialogButton = new QPushButton("Tabela rozwiązań Pareto");
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setDisabled(true);
    timeSlider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    expression1 = new QLineEdit;
    expression1->setFixedWidth(FUNCTION_INPUT_WIDTH);
    expression1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    expression2 = new QLineEdit;
    expression2->setFixedWidth(FUNCTION_INPUT_WIDTH);
    expression2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    functionType1 = new QComboBox;
    functionType1->setFixedWidth(FUNCTION_INPUT_WIDTH);
    functionType2 = new QComboBox;
    functionType2->setFixedWidth(FUNCTION_INPUT_WIDTH);
    for(const auto& x : functionNames){
        functionType1->addItem(x);
        functionType2->addItem(x);
    }
    functionType1->setCurrentIndex(0);
    functionType2->setCurrentIndex(0);

    customFunctions = new QRadioButton("Wybór funkcji celu");
    customFunctions->setChecked(true);
    preparedFunctions = new QRadioButton("Gotowe funkcje testowe");
    preparedType = new QComboBox;
    //preparedType->setFixedWidth(FUNCTION_INPUT_WIDTH);
    for(const auto& x : testNames)
        preparedType->addItem(x);
    preparedType->setCurrentIndex(0);

    fixedScale = new QCheckBox("Zablokuj skalę");

    for(int i=0; i<MAX_PROBLEM_SIZE; ++i)
        solutionRange[i] = std::make_pair(-DEFAULT_X_RANGE,DEFAULT_X_RANGE);

    plot = new QCustomPlot;
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);
    plot->addGraph();
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,3));
    plot->xAxis->setLabel("f1(x)");
    plot->xAxis->setLabelFont(mathFont);
    plot->yAxis->setLabel("f2(x)");
    plot->yAxis->setLabelFont(mathFont);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, "Zbiór Pareto w przestrzeni funkcyjnej", QFont("sans", 12, QFont::Bold)));

    connect(populationSize,SIGNAL(valueChanged(int)),nsga,SLOT(setPopulationSize(int)));
    connect(problemSize,SIGNAL(valueChanged(int)),nsga,SLOT(setProblemSize(int)));
    connect(generationBox,SIGNAL(valueChanged(int)),this,SLOT(setNumberOfGenerations(int)));
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(start()) );
    connect(rangeDialogButton,SIGNAL(clicked(bool)),this,SLOT(openRangeDialog()));
    connect(paretoDialogButton,SIGNAL(clicked(bool)),this,SLOT(openTableDialog()));
    connect(timeSlider,SIGNAL(valueChanged(int)),this,SLOT(displayScene(int)));
    connect(functionType1, SIGNAL(currentIndexChanged(int)),nsga,SLOT(setObjType1(int)));
    connect(functionType2, SIGNAL(currentIndexChanged(int)),nsga,SLOT(setObjType2(int)));
    connect(functionType1, SIGNAL(currentIndexChanged(int)),this,SLOT(disableExpression1(int)));
    connect(functionType2, SIGNAL(currentIndexChanged(int)),this,SLOT(disableExpression2(int)));
    connect(preparedFunctions,SIGNAL(toggled(bool)),nsga,SLOT(activateTestFunction(bool)));
    connect(preparedType,SIGNAL(currentIndexChanged(int)),nsga,SLOT(setTestFunctionType(int)));
    connect(preparedType,SIGNAL(currentIndexChanged(int)),this,SLOT(adjustProblemSize()));
    connect(preparedFunctions,SIGNAL(clicked(bool)),this, SLOT(adjustProblemSize()));

    QFormLayout* paramLayout = new QFormLayout;
    paramLayout->addRow("Rozmiar populacji:",populationSize);
    paramLayout->addRow("Wymiar problemu:",problemSize);
    paramLayout->addRow("Liczba generacji:",generationBox);

    QGroupBox* objectiveGroupBox = new QGroupBox("Funkcje celu");
    QVBoxLayout* objectiveLayout = new QVBoxLayout;
    exprLayout = new QFormLayout;
    exprLayout->addRow("Typ f1(x)", functionType1);
    exprLayout->addRow("f1(x) = ", expression1);
    exprLayout->addRow("Typ f2(x)", functionType2);
    exprLayout->addRow("f2(x) = ", expression2);
    objectiveLayout->addWidget(customFunctions);
    objectiveLayout->addLayout(exprLayout);
    objectiveLayout->addWidget(preparedFunctions);
    QFormLayout* preparedLayout = new QFormLayout;
    preparedLayout->addRow("Typ:",preparedType);
    objectiveLayout->addLayout(preparedLayout);
    objectiveGroupBox->setLayout(objectiveLayout);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    QGroupBox* paramGroupBox = new QGroupBox("Parametry");
    paramGroupBox->setLayout(paramLayout);
    rightLayout->addWidget(paramGroupBox);
    rightLayout->addWidget(objectiveGroupBox);
    rightLayout->addWidget(rangeDialogButton);
    rightLayout->addWidget(paretoDialogButton);
    rightLayout->addWidget(startButton);
    rightLayout->addStretch();
    rightLayout->addWidget(generationCounter);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(fixedScale);
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

    createMenus();
}

MainWindow::~MainWindow()
{

}
