#ifndef REGMODEL_H
#define REGMODEL_H

#define ROWCOUNT 16
#define COLUMNCOUNT 16
#define BANK1_ADR 0x80
#define BANK1 BANK1_ADR/COLUMNCOUNT


#include <QAbstractTableModel>
#include "adressen.h"


class RegModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RegModel(QObject *parent= 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;



    ~RegModel();

    int reg[2][128];
    int getIDX(int row, int column) const;
    QStringList vertHeader;
    QStringList Header;

    void updateOtherBank(int bank, int idx);
};

#endif // REGMODEL_H

