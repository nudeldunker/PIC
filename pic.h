#ifndef PIC_H
#define PIC_H

#include <QObject>

class PIC : public QObject
{
    Q_OBJECT
public:
    explicit PIC(QObject *parent = 0);
    ~PIC();

signals:

public slots:
};

#endif // PIC_H
