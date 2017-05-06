#ifndef RANGE_DIALOG_H
#define RANGE_DIALOG_H

#include <QDialog>
#include <array>
#include <utility>
#include <vector>
#include "constants.h"

class QSpinBox;

class RangeDialog: public QDialog{
    Q_OBJECT

    std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>* range;
    std::vector<QSpinBox*> minRanges;
    std::vector<QSpinBox*> maxRanges;
    int size;

private slots:
    void save();

public:
    RangeDialog(std::array<std::pair<double,double>, MAX_PROBLEM_SIZE>* r, int sz, QWidget* parent);

};

#endif // RANGE_DIALOG_H
