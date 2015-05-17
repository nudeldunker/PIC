#include "hdwcom.h"
#include <QDebug>
#include <QSerialPortInfo>

HDWCom::HDWCom(QString portName)
{
    this->portName=portName;
}

bool HDWCom::open()
{
    if(port!=0)
    {
        delete port;
    }

    if(!QSerialPortInfo(portName).isNull())
    {
        port=new QSerialPort(portName);
        if(port->isOpen())
        {
            delete port;
            return false;
        }

        port->open(QIODevice::ReadWrite);

        port->setBaudRate(QSerialPort::Baud4800);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);

        if(port->error()!=QSerialPort::NoError)
        {
            qDebug()<<port->error();
            delete port;
            return false;
        }
        return true;
    }
    return false;

}

bool HDWCom::close()
{
    if(port->isOpen())
        port->close();

}

void HDWCom::encode()
{

}

void HDWCom::decode()
{

}
