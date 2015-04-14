#include "pic.h"

PIC::PIC(QObject *parent) : QObject(parent)
{

}

PIC::~PIC()
{

}

void PIC::decodeCmd()
{


    for( int i=0; i<m_CmdList.size(); i++)
    {
     int k_long=m_CmdList[i] & 0x7FF;
     int k=m_CmdList[i] & 0xFF;
     int f=m_CmdList[i] & 0x7F;
     int d=m_CmdList[i] & 0x80;
     int l=d;
     int b=m_CmdList[i] & 0x380;

     int ByteCmd=m_CmdList[i] & 0x3F00;
     int BitCmd=m_CmdList[i] & 0x3C00;
     int ShrtCmd=m_CmdList[i] & 0x3800;

     if(ByteCmd == 0x0700 )
        qDebug() << "ADDWF" <<i; //ADDWF(f,d);
     else if(ByteCmd == 0x0500)
         qDebug() << "ANDWF(f,d)" << i;
     else if(ByteCmd == 0x0100)
         CLRF(f);
     else if((m_CmdList[i] & 0x3F80) == 0x0100)
         CLRW();
     else if(ByteCmd == 0x0900)
         COMF(f,d);
     else if(ByteCmd == 0x0300)
         DECF(f,d);
     else if(ByteCmd == 0x0B00)
         DECFSZ(f,d);
     else if(ByteCmd == 0x0A00)
         INCF(f,d);
     else if(ByteCmd == 0x0F00)
         INCFSZ(f,d);
     else if(ByteCmd == 0x0400)
         IORWF(f,d);
     else if(ByteCmd == 0x0800)
         MOVF(f,d);
     else if(ByteCmd == 0x0000)
         MOVWF(f);
     else if((m_CmdList[i] & 0x3F9F) == 0x0000)
         NOP();
     else if(ByteCmd == 0x0D00)
         RLF(f,d);
     else if(ByteCmd == 0x0C00)
         RRF(f,d);
     else if(ByteCmd == 0x0200)
         SUBWF(f,d);
     else if(ByteCmd == 0x0E00)
         SWAPF(f,d);
     else if(ByteCmd == 0x0600)
         XORWF(f,d);

     else if(BitCmd == 0x1000)
         BCF(f,b);
     else if(BitCmd == 0x1400)
         BSF(f,b);
     else if(BitCmd == 0x1800)
         BTFSC(f,b);
     else if(BitCmd == 0x1C00)
         BTFSS(f,b);

     else if((m_CmdList[i] & 0x3E00 ) == 0x3E00)
        ADDLW(k);
     else if(ByteCmd == 0x3900)
         ANDLW(k);
     else if(ShrtCmd == 0x2000)
         CALL(k_long);
     else if((m_CmdList[i]& 0XFFFF) == 0x0064)
         CLRWDT();
     else if(ShrtCmd == 0x2800)
         GOTO(k_long);

//




    if((m_CmdList[i] & 0x3F00 ) == 0x3A00)
        qDebug()<< "XORLW" << i;

    if((m_CmdList[i] & 0x3E00 ) == 0x3C00)
        qDebug()<< "SUBLW" << i;

    if((m_CmdList[i] & 0x3F00 ) == 0x3C00)
        qDebug()<< "SUBLW" << i;

    if((m_CmdList[i] & 0xFFFF ) == 0x0063)
        qDebug()<< "SLEEP" << i;

    if((m_CmdList[i] & 0xFFFF ) == 0x0008)
        qDebug()<< "RETURN" << i;

    if((m_CmdList[i] & 0x3C00 ) == 0x3400)
        qDebug()<< "RETURNLW" << i;

    if((m_CmdList[i] & 0xFFFF ) == 0x0009)
        qDebug()<< "RETURNFIE" << i;

    if((m_CmdList[i] & 0x3F00 ) == 0x3000)
        qDebug()<< "MOVLW" << i;

    if((m_CmdList[i] & 0x3F00 ) == 0x3100)
        qDebug()<< "MOVLW" << i;

    if((m_CmdList[i] & 0x3F00 ) == 0x3200)
        qDebug()<< "MOVLW" << i;

    if((m_CmdList[i] & 0x3F00 ) == 0x3300)
        qDebug()<< "MOVLW" << i;


    if((m_CmdList[i] & 0x3800 ) == 0x3800)
        qDebug()<< "IORLW" << i;
}
}
