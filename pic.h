#ifndef PIC_H
#define PIC_H

#include <QObject>
#include "regmodel.h"

class PIC : public QObject
{
    Q_OBJECT
public:
    explicit PIC(QObject *parent = 0);
    ~PIC();

    RegModel *regmodel;
    QList<int> m_CmdList;




    void decodeCmd();
signals:

public slots:
};

#endif // PIC_H
