#ifndef REGMODELDLGT_H
#define REGMODELDLGT_H

#include <QStyledItemDelegate>


class RegModelDlgt : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RegModelDlgt(QWidget *parent=0);
    ~RegModelDlgt();


    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget * editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem & option, const QModelIndex &index)const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

#endif // REGMODELDLGT_H
