#ifndef HDWCOM_H
#define HDWCOM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class HDWCom
{
public:
    HDWCom(QString portName);

    QSerialPort * port=0;
    QString portName=0;
};

#endif // HDWCOM_H
