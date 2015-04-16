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
        ADDWF(f,d);
     else if(ByteCmd == 0x0500)
        ANDWF(f,d);
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
     else if((ByteCmd) == 0x3A00)
        XORLW();

     else if((m_CmdList[i] & 0x3E00 ) == 0x3C00)
        SUBLW();

     else if((ByteCmd) == 0x3C00)
        SUBLW();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0063)
        SLEEP();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0008)
        RETURN();

    else if((BitCmd) == 0x3400)
        RETURNLW();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0009)
        RETURNFIE();

    else if((ByteCmd) == 0x3000)
        MOVLW();

    else if((ByteCmd) == 0x3100)
        MOVLW();

    else if((ByteCmd) == 0x3200)
        MOVLW();

    else if((ByteCmd) == 0x3300)
        MOVLW();


    else if((ShrtCmd) == 0x3800)
        IORLW();
}
}


void PIC::ADDWF(){qDebug << ADDWF;}
void PIC::ANDWF(){qDebug << ANDWF;}
void PIC::CLRF(){qDebug << CLRF;}
void PIC::CLRW(){qDebug << CLRW;}
void PIC::COMF(f,d){qDebug << COMF;}
void PIC::DECF(f,d){qDebug << DECF;}
void PIC::DECFSZ(f,d){qDebug << DECFSZ;}
void PIC::INCF(f,d){qDebug << INCF;}
void PIC::INCFSZ(f,d){qDebug << INCFSZ;}
void PIC::IORWF(f,d){qDebug << IORWF;}
void PIC::MOVF(f,d){qDebug << MOVF;}
void PIC::MOVWF(f){qDebug << MOVWF;}
void PIC::NOP(){qDebug << NOP;}
void PIC::RLF(f,d){qDebug << RLF;}
void PIC::RRF(f,d){qDebug << RRF;}
void PIC::SUBWF(f,d){qDebug << SUBWF;}
void PIC::SWAPF(f,d){qDebug << SWAPF;}
void PIC::XORWF(f,d){qDebug << XORWF;}
void PIC::BCF(f,b){qDebug << BCF;}
void PIC::BSF(f,b){qDebug << BSF;}
void PIC::BTFSC(f,b){qDebug << BTFSC;}
void PIC::BTFSS(f,b){qDebug << BTFSS;}
void PIC::ADDLW(k){qDebug << ADDLW;}
void PIC::ANDLW(k){qDebug << ANDLW;}
void PIC::CALL(k_long){qDebug << CALL;}
void PIC::CLRWDT(){qDebug << CLRWDT;}
void PIC::GOTO(k_long){qDebug << GOTO;}
void PIC::XORLW(){qDebug << XORLW;}
void PIC::SUBLW(){qDebug << SUBLW;}
void PIC::SUBLW(){qDebug << SUBLW;}
void PIC::SLEEP(){qDebug << SLEEP;}
void PIC::RETURN(){qDebug << RETURN;}
void PIC::RETURNLW(){qDebug << RETURNLW;}
void PIC::RETURNFIE(){qDebug << RETURNFIE;}
void PIC::MOVLW(){qDebug << MOVLW;}
void PIC::MOVLW(){qDebug << MOVLW;}
void PIC::MOVLW(){qDebug << MOVLW;}
void PIC::MOVLW(){qDebug << MOVLW;}
void PIC::IORLW(){qDebug << IORLW;}
