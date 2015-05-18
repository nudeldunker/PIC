#include "portmodel.h"
#include <math.h>
#include <QDebug>



PortModel::PortModel(QObject *parent, int adresse, RegModel *regAdr)
{
   adr=adresse;
    Header << "7" << "6" <<"5"<<"4"<<"3"<<"2"<<"1"<<"0";
    vertHeader <<"Pin"<<"Tris";

   regModel=regAdr;
}

PortModel::~PortModel()
{

}

QModelIndex PortModel::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        return QModelIndex();
    }

    if( (0 <= row) && (row < rowCount()) &&
            (0 <= column) && (column < columnCount())) {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex PortModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int PortModel::rowCount(const QModelIndex &parent) const {
    return vertHeader.size();
}

int PortModel::columnCount(const QModelIndex &parent) const {
    return Header.size();
}

QVariant PortModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(role==Qt::DisplayRole)
        {
            return QString::number((regModel->reg[index.row()][adr] & qRound(pow(2,7-index.column())))>>7-index.column()); //TODO in hex ausgeben
        }
        if(role==Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        else return QVariant();
    }
    else return QVariant();

}

QVariant PortModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole && section < columnCount())
        return Header[section];
    else if(orientation==Qt::Vertical && role==Qt::DisplayRole && section <rowCount())
        return vertHeader[section];
    else return QVariant();

}

Qt::ItemFlags PortModel::flags(const QModelIndex& index ) const {
    if(index.isValid() && index.row()==1) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    }
    else return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return QAbstractItemModel::flags(index);
}

bool PortModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // validate the index and that row and column are inside the given borders...
    if(!index.isValid() || index.row()>rowCount() || index.column()>columnCount()) {
        return false;
    }


    if(Qt::EditRole == role && index.row()==0) {
        // change the value of the model
       if(((regModel->reg[0][adr] & qRound( pow(2, 7-index.column())))>>7-index.column())==1)
       { regModel->reg[0][adr] = regModel->reg[0][adr] & ~qRound(pow(2,7-index.column()));

       }
       else
       { regModel->reg[0][adr] = regModel->reg[0][adr] |qRound(pow(2, 7-index.column()));
           qDebug()<< qRound(pow(2,7-index.column()));
           qDebug()<< (regModel->reg[0][adr] | qRound(pow(2,7-index.column())));
       }

        emit dataChanged(index, index);
        regModel->dataChanged(QModelIndex(), QModelIndex());
        return true;
    }

    return false;
}



