#ifndef CODEMODEL_H
#define CODEMODEL_H

#include <QAbstractTableModel>


class CodeModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    CodeModel(QObject *parent= 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    int findPCIdx(int pc);

    ~CodeModel();

    QVector<QStringList> code;
    int cols=3;

    QStringList Header;
};

#endif // CODEMODEL_H
