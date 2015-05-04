#include "codemodel.h"

#include <QDebug>

#define PCounter 1  // PC steht in Spalte 1



CodeModel::CodeModel(QObject *parent) :
  QAbstractTableModel(parent)
{
    Header.push_back("BP");
    Header.push_back("PC");
    Header.push_back("Text");
}

CodeModel::~CodeModel()
{

}

QModelIndex CodeModel::index(int row, int column, const QModelIndex &parent) const {
    if(parent.isValid()) {
        return QModelIndex();
    }

    if( (0 <= row) && (row < rowCount()) &&
            (0 <= column) && (column < columnCount())) {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex CodeModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int CodeModel::rowCount(const QModelIndex &parent) const {
    return code.size();
}

int CodeModel::columnCount(const QModelIndex &parent) const {
    return cols;
}

QVariant CodeModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(role==Qt::DisplayRole)
        {
            return code[index.row()][index.column()]; //---------------------------------------
        }

        if(role==Qt::TextAlignmentRole)
        {
            return Qt::AlignLeft;
        }
        else return QVariant();
    }
    else return QVariant();

}

QVariant CodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole && section < columnCount())
        return Header[section];
    else return QVariant();

}

bool CodeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // validate the index and that row and column are inside the given borders...
    if(!index.isValid() || index.row()>rowCount() || index.column()>columnCount()) {
        return false;
    }


    if(Qt::EditRole == role && index.column()==0) {
        // change the value of the model

        if(code[index.row()][index.column()].isEmpty())
            code[index.row()][index.column()]="B";
        else code[index.row()][index.column()].clear();

        emit dataChanged(index, index);
        qDebug()<<"BP verändert?";

        //emit dataChanged(index, index);
        return true;

    }

    return false;
}

Qt::ItemFlags CodeModel::flags(const QModelIndex& index ) const {
    if(index.isValid()) {

        if(index.column()==0)
        {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );

        }
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    }

    return QAbstractItemModel::flags(index);
}

int CodeModel::findPCIdx(int pc)
{
    for( int i=0; i< this->rowCount(); i++)
    {
        if(code[i][PCounter].toInt(0,16)==pc)
        {
            qDebug()<< "PCounter : " <<pc;
            qDebug() << "Ausgewählte Zeile: " <<i;
            return i;
        }
    }

    return -1;
}



