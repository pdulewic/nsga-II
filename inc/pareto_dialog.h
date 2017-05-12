#ifndef PARETO_DIALOG_H
#define PARETO_DIALOG_H

#include <QDialog>
#include <vector>
#include "solution.h"
class QTableView;
class ParetoTable;
class QPushButton;

class ParetoDialog: public QDialog{
    QTableView* tableView;
    ParetoTable* paretoTable;
    QPushButton* closeButton;
public:
    ParetoDialog(QWidget *parent = nullptr);
    void setTable(std::vector<Solution> tab);

};

#endif // PARETO_DIALOG_H
