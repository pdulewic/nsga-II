#ifndef PARETO_TABLE_H
#define PARETO_TABLE_H

#include "solution.h"
#include <vector>
#include <QAbstractTableModel>

class ParetoTable: public QAbstractTableModel{
    std::vector<Solution> table;
    int problemSize;
public:
    ParetoTable( std::vector<Solution> t, QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
};

#endif // PARETO_TABLE_H
