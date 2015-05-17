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
    bool close();
    void encode();
    void decode();
    bool open();
};

#endif // HDWCOM_H
