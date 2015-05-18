#ifndef REGAMODEL_H
#define REGAMODEL_H

#include <QAbstractTableModel>
#include "regmodel.h"


class PortModel : public QAbstractTableModel
{    Q_OBJECT
 public:
     PortModel(QObject *parent= 0, int adresse=0, RegModel* regAdr=0);

     QModelIndex index(int row, int column, const QModelIndex &parent) const;
     QModelIndex parent(const QModelIndex &child) const;

     int rowCount(const QModelIndex &parent=QModelIndex()) const;
     int columnCount(const QModelIndex &parent=QModelIndex()) const;

     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;

     bool setData(const QModelIndex &index, const QVariant &value, int role);
     Qt::ItemFlags flags(const QModelIndex &index) const;

     int reg[2][8];
     int adr;
     QStringList Header;
     QStringList vertHeader;

     RegModel *regModel;

     ~PortModel();
};

#endif // REGAMODEL_H
