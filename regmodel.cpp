#include "regmodel.h"

RegModel::RegModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

RegModel::~RegModel()
{

}

QModelIndex DataModel::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        return QModelIndex();
    }

    if( (0 <= row) && (row < rowCount()) &&
            (0 <= column) && (column < columnCount())) {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex DataModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int DataModel::rowCount(const QModelIndex &parent) const {
    return rowCount;
}

int DataModel::columnCount(const QModelIndex &parent) const {
    return columnCount;
}

QVariant RegModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    int bank;
    if(index.row()< )
        bank=0;
    else bank=1;

    idx=getIDX(index.row(), index.column());

    return reg[bank][idx]; //TODO in hex ausgeben

}

Qt::ItemFlags DataModel::flags(const QModelIndex& index ) const {
    if(index.isValid()) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    }

    return QAbstractItemModel::flags(index);
}

bool RegModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // validate the index and that row and column are inside the given borders...
    if(!index.isValid() || index.row()>rowCount(index) || index.column()>columnCount(index)) {
        return false;
    }


    if(Qt::EditRole == role) {
        // change the value of the model
        int bank;
        if(index.row()< )
            bank=0;
        else bank=1;

        int idx=getIDX(index.row(), index.column());

        reg[bank][idx]=value.toString().toInt(0,16); //TODO direkt in Hex

        // don't forget to emit the signal dataChanged and to return true
        emit dataChanged(index, index);
        return true;
    }

    // in case, no editing was done, return false.
    return false;
}

int RegModel::getIDX(int row, int column)
{

    idx=row*columnCount+column;
    if(idx>127) idx-=128;
    qDebug()<< "Alter Index : Reihe: " <<row <<"Spalte" <<column;
    qDebug()<< "Neuer Index : "<<idx;
    return idx;
}

//rowCount und ColumnCount als defines? -> Wert für bank Abfrage auch als Makro



