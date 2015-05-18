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
    for(int i=0; i<4; i++)
    {
        int nibble1=(portData[i]&0xF0)>>4;
        nibble1=nibble1|0x30;

        int nibble2=(portData[i]&0x0F);
        nibble2=nibble2|0x30;

        sendData[2*i]=nibble1;
        sendData[(2*i)+1]=nibble2;
    }
    sendData[8]=0x0d;
}

int HDWCom::decode(char nibble1, char nibble2)
{
    int erg=((nibble1&0x0F)<<4);
    erg= erg|((nibble2&0x0F));
    return erg;

}

void HDWCom::readData()
{
    QByteArray data=port->readLine(64);

    int j=0;
    for(int i=0; i<data.size();i++)
    {
        newData[j]=decode(data[i], data[i+1]);
        i++;
        j++;
    }

    emit newPortData();


}

void HDWCom::writeData()
{
    if(readyToWrite)
    {
    emit getData();
    encode();
    int send=port->write(sendData);
    if(send==-1 || send!=sendData.size())
    {
        qDebug()<<"Fehler beim Senden";
    }
    readyToWrite=false;
    }

}

void HDWCom::setReadyToWrite()
{
    readyToWrite=true;
}


