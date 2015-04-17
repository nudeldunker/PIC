#include "regmodeldlgt.h"

#include <QLineEdit>

RegModelDlgt::RegModelDlgt(QWidget *parent)
    :QStyledItemDelegate(parent)
{

}

RegModelDlgt::~RegModelDlgt()
{

}

void RegModelDlgt::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize RegModelDlgt::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget * RegModelDlgt::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QValidator *validator= new QIntValidator(0,0xFF, parent);
    QLineEdit *leditor= new QLineEdit(parent);

    leditor->setValidator(validator);

    return leditor;
}

void RegModelDlgt::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(QLineEdit *leditor=qobject_cast<QLineEdit*>(editor))
    {
       leditor->setText(index.model()->data(index, Qt::EditRole).toString());
    }
}

void RegModelDlgt::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void RegModelDlgt::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( QLineEdit *leditor=qobject_cast<QLineEdit*>(editor))
    {
        model->setData(index, leditor->text(), Qt::EditRole);
    }
}
