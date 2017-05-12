#include "inc/pareto_dialog.h"
#include "inc/pareto_table.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>

ParetoDialog::ParetoDialog(QWidget *parent): QDialog(parent), paretoTable(nullptr) {
    tableView = new QTableView;
    closeButton = new QPushButton("Zamknij");
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(tableView);
    layout->addLayout(buttonLayout);
    setLayout(layout);
}

void ParetoDialog::setTable(std::vector<Solution> tab){
    delete paretoTable;
    paretoTable = new ParetoTable(tab);
    tableView->setModel(paretoTable);
    adjustSize();
}
