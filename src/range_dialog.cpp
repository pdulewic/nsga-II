#include "inc/range_dialog.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

void RangeDialog::save(){
    for(int i=0; i<size; ++i){
        if(minRanges[i]->value() < maxRanges[i]->value())
            range->at(i) = std::make_pair(minRanges[i]->value(),maxRanges[i]->value());
        else{
            QMessageBox errorBox(QMessageBox::Warning, "Uwaga!", "Minimalna wartość zmiennej x" +
                                 QString::number(i+1) + " jest większa lub równa wartości maksymalnej", 0, this);
            errorBox.exec();
            return;
        }
    }
    close();
}

RangeDialog::RangeDialog(std::array<std::pair<double, double>, MAX_PROBLEM_SIZE> *r, int sz, QWidget *parent = nullptr): QDialog(parent), range(r), size(sz){
    setWindowTitle("Ograniczenia elementów wektora x");
    QGridLayout* layout = new QGridLayout;
    for(int i = 0; i<size; ++i){
        layout->addWidget(new QLabel("x" + QString::number(i+1)),i,0);

        layout->addWidget(new QLabel("Wartość minimalna:"),i,1);
        minRanges.push_back(new QSpinBox);
        minRanges[i]->setRange(MIN_X_RANGE,MAX_X_RANGE);
        minRanges[i]->setValue(range->at(i).first);
        layout->addWidget(minRanges[i],i,2);

        layout->addWidget(new QLabel("Wartość maksymalna:"),i,3);
        maxRanges.push_back(new QSpinBox);
        maxRanges[i]->setRange(MIN_X_RANGE,MAX_X_RANGE);
        maxRanges[i]->setValue(range->at(i).second);
        layout->addWidget(maxRanges[i],i,4);
    }

    QPushButton* acceptButton = new QPushButton("Zapisz");
    QPushButton* closeButton = new QPushButton("Zamknij");

    connect(acceptButton,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}
