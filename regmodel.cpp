#include "regmodel.h"
#include "adressen.h"

#include <QDebug>

RegModel::RegModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    for (int i=0; i<columnCount(); i++)
        Header.push_back(QString::number(i, 16).toUpper());

    for (int i=0; i<rowCount(); i++)
        vertHeader.push_back(QString::number(i*columnCount(), 16).toUpper());

    memset(reg,0, sizeof(reg));

    //TODO Startwerte der Register setzen

}

RegModel::~RegModel()
{

}

QModelIndex RegModel::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        return QModelIndex();
    }

    if( (0 <= row) && (row < rowCount()) &&
            (0 <= column) && (column < columnCount())) {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex RegModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int RegModel::rowCount(const QModelIndex &parent) const {
    return ROWCOUNT;
}

int RegModel::columnCount(const QModelIndex &parent) const {
    return COLUMNCOUNT;
}

QVariant RegModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(role==Qt::DisplayRole)
        {
            int bank;
            if(index.row()< BANK1)
                bank=0;
            else bank=1;

            int idx=getIDX(index.row(), index.column());

            return QString::number(reg[bank][idx], 16).toUpper(); //TODO in hex ausgeben
        }
        if(role==Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        else return QVariant();
    }
    else return QVariant();

}

QVariant RegModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole && section < columnCount())
        return Header[section];
    else if(orientation==Qt::Vertical && role==Qt::DisplayRole && section <rowCount())
        return vertHeader[section];
    else return QVariant();

}

Qt::ItemFlags RegModel::flags(const QModelIndex& index ) const {
    if(index.isValid()) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    }

    return QAbstractItemModel::flags(index);
}

bool RegModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // validate the index and that row and column are inside the given borders...
    if(!index.isValid() || index.row()>rowCount() || index.column()>columnCount()) {
        return false;
    }


    if(Qt::EditRole == role) {
        // change the value of the model
        int bank;
        if(index.row()< BANK1)
            bank=0;
        else bank=1;

        int idx=getIDX(index.row(), index.column());

        reg[bank][idx]=value.toString().toInt(0,16); //TODO direkt in Hex

        if(idx==STATUS || idx==PCL || idx== PCLATH || idx==INTCON || idx==FSR)
        {
            updateOtherBank(bank, idx);
        }

        //emit dataChanged(index, index);
        return true;
    }

    return false;
}

int RegModel::getIDX(int row, int column) const
{

    int idx=row*columnCount()+column;
    if(idx>=BANK1_ADR) idx-=BANK1_ADR;
//    qDebug()<< "Alter Index : Reihe: " <<row <<"Spalte" <<column;
//    qDebug()<< "Neuer Index : "<<idx;
    return idx;
}

void RegModel::updateOtherBank(int bank, int idx)
{
    int bank1;
    if(bank==1) bank1=0; else bank1=1;

    reg[bank1][idx]=reg[bank][idx];
}

//rowCount und ColumnCount als defines? -> Wert für bank Abfrage auch als Makro



