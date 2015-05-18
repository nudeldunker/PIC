#ifndef HDWCOM_H
#define HDWCOM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>


class HDWCom: public QObject
{
    Q_OBJECT
public:
    HDWCom(QString portName);

    QSerialPort * port=0;
    QString portName=0;

    int newData[3];

    int portData[4];
    QByteArray sendData;

    bool readyToWrite=true;
    bool close();
    void encode();
    bool open();
    int decode(char nibble1, char nibble2);
public slots:
    void readData();

    void writeData();
    void setReadyToWrite();
signals:
    void newPortData();
    void getData();
};

#endif // HDWCOM_H
