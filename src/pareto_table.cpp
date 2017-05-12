#include "inc/pareto_table.h"

ParetoTable::ParetoTable(std::vector<Solution> t, QObject *parent): QAbstractTableModel(parent), table(t) {
    if(table.empty())
        problemSize = 0;
    else
        problemSize = table[0].val.size();
}

int ParetoTable::rowCount(const QModelIndex &) const{
    return table.size();
}

int ParetoTable::columnCount(const QModelIndex &) const{
    return problemSize + 2;
}

QVariant ParetoTable::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        int row = index.row();
        int column = index.column();
        double value;
        if(column == 0)
            value = table[row].objValue1;
        else if(column == 1)
            value = table[row].objValue2;
        else
            value = table[row].val[column-2];
        return QVariant(QString::number(value));
    }
    return QVariant();
}

QVariant ParetoTable::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role == Qt::DisplayRole){
        switch(orientation){
        case Qt::Horizontal:
            if(section == 0)
                return QVariant("f1(x)");
            else if(section == 1)
                return QVariant("f2(x)");
            else
                return QVariant("x" + QString::number(section - 1));
            break;
        case Qt::Vertical:
            return QVariant(QString::number(section+1));
        }
    }
    return QVariant();
}
