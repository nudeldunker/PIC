#ifndef REGMODEL_H
#define REGMODEL_H

#include <QAbstractTableModel>


class RegModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RegModel(QObject *parent= 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);


    ~RegModel();

    int reg[2][127];
    int rowCount = 16;
    int columnCount = 16;
    int getIDX(int row, int column);

};

#endif // REGMODEL_H
