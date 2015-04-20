#include "pic.h"
#include "QDebug"

PIC::PIC(QObject *parent) : QObject(parent)
{
    regModel=new RegModel(parent);
    regModelDlgt= new RegModelDlgt(qobject_cast<QWidget*>(parent));

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
        ADDWF();
     else if(ByteCmd == 0x0500)
        ANDWF();
     else if(ByteCmd == 0x0100)
        CLRF();
     else if((m_CmdList[i] & 0x3F80) == 0x0100)
        CLRW();
     else if(ByteCmd == 0x0900)
        COMF();
     else if(ByteCmd == 0x0300)
        DECF();
     else if(ByteCmd == 0x0B00)
        DECFSZ();
     else if(ByteCmd == 0x0A00)
        INCF();
     else if(ByteCmd == 0x0F00)
        INCFSZ();
     else if(ByteCmd == 0x0400)
        IORWF();
     else if(ByteCmd == 0x0800)
        MOVF();
     else if(ByteCmd == 0x0000)
        MOVWF();
     else if((m_CmdList[i] & 0x3F9F) == 0x0000)
        NOP();
     else if(ByteCmd == 0x0D00)
        RLF();
     else if(ByteCmd == 0x0C00)
        RRF();
     else if(ByteCmd == 0x0200)
        SUBWF();
     else if(ByteCmd == 0x0E00)
        SWAPF();
     else if(ByteCmd == 0x0600)
        XORWF();
     else if(BitCmd == 0x1000)
        BCF();
     else if(BitCmd == 0x1400)
        BSF();
     else if(BitCmd == 0x1800)
        BTFSC();
     else if(BitCmd == 0x1C00)
        BTFSS();
     else if((m_CmdList[i] & 0x3E00 ) == 0x3E00)
        ADDLW();
     else if(ByteCmd == 0x3900)
        ANDLW();
     else if(ShrtCmd == 0x2000)
        CALL();
     else if((m_CmdList[i]& 0XFFFF) == 0x0064)
        CLRWDT();
     else if(ShrtCmd == 0x2800)
        GOTO();
     else if((ByteCmd) == 0x3A00)
        XORLW();

     else if((m_CmdList[i] & 0x3E00 ) == 0x3C00)
        SUBLW1();

     else if((ByteCmd) == 0x3C00)
        SUBLW2();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0063)
        SLEEP();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0008)
        RETURN();

    else if((BitCmd) == 0x3400)
        RETURNLW();

    else if((m_CmdList[i] & 0xFFFF ) == 0x0009)
        RETURNFIE();

    else if((ByteCmd) == 0x3000)
        MOVLW1();

    else if((ByteCmd) == 0x3100)
        MOVLW2();

    else if((ByteCmd) == 0x3200)
        MOVLW3();

    else if((ByteCmd) == 0x3300)
        MOVLW4();


    else if((ShrtCmd) == 0x3800)
        IORLW();
}
}


void PIC::ADDWF(){qDebug() << "ADDWF";}
void PIC::ANDWF(){qDebug() << "ANDWF";}
void PIC::CLRF(){qDebug() << "CLRF";}
void PIC::CLRW(){qDebug() << "CLRW";}
void PIC::COMF(){qDebug() << "COMF";}
void PIC::DECF(){qDebug() << "DECF";}
void PIC::DECFSZ(){qDebug() << "DECFSZ";}
void PIC::INCF(){qDebug() << "INCF";}
void PIC::INCFSZ(){qDebug() << "INCFSZ";}
void PIC::IORWF(){qDebug() << "IORWF";}
void PIC::MOVF(){qDebug() << "MOVF";}
void PIC::MOVWF(){qDebug() << "MOVWF";}
void PIC::NOP(){qDebug() << "NOP";}
void PIC::RLF(){qDebug() << "RLF";}
void PIC::RRF(){qDebug() << "RRF";}
void PIC::SUBWF(){qDebug() << "SUBWF";}
void PIC::SWAPF(){qDebug() << "SWAPF";}
void PIC::XORWF(){qDebug() << "XORWF";}
void PIC::BCF(){qDebug() << "BCF";}
void PIC::BSF(){qDebug() << "BSF";}
void PIC::BTFSC(){qDebug() << "BTFSC";}
void PIC::BTFSS(){qDebug() << "BTFSS";}
void PIC::ADDLW(){qDebug() << "ADDLW";}
void PIC::ANDLW(){qDebug() << "ANDLW";}
void PIC::CALL(){qDebug() << "CALL";}
void PIC::CLRWDT(){qDebug() << "CLRWDT";}
void PIC::GOTO(){qDebug() << "GOTO";}
void PIC::XORLW(){qDebug() << "XORLW";}
void PIC::SUBLW1(){qDebug() << "SUBLW1";}
void PIC::SUBLW2(){qDebug() << "SUBLW2";}
void PIC::SLEEP(){qDebug() << "SLEEP";}
void PIC::RETURN(){qDebug() << "RETURN";}
void PIC::RETURNLW(){qDebug() << "RETURNLW";}
void PIC::RETURNFIE(){qDebug() << "RETURNFIE";}
void PIC::MOVLW1(){qDebug() << "MOVLW1";}
void PIC::MOVLW2(){qDebug() << "MOVLW2";}
void PIC::MOVLW3(){qDebug() << "MOVLW3";}
void PIC::MOVLW4(){qDebug() << "MOVLW4";}
void PIC::IORLW(){qDebug() << "IORLW";}



void PIC::CBit(bool set)
{
    if(set)
        this->regModel->reg[STATUS] |= 0x1;
    else
        this->regModel->reg[STATUS] &= 0xFE;
}

void PIC::DCBit(bool set)
{
    if(set)
        this->regModel->reg[STATUS] |=0x2;
    else
        this->regModel->reg[STATUS] &= 0xFD;
}

void PIC::ZBit(bool set)
{
    if(set)
        this->regModel->reg[STATUS] |= 0x4;
    else
        this->regModel->reg[STATUS] &= 0xFB;

}

void PIC::PDBit (bool set)
{
    if(set)
        this->regModel->reg[STATUS] |= 0x8;
    else
        this->regModel->reg[STATUS] &= 0xF7;

}

void PIC::TOBit (bool set)
{
    if(set)
        this->regModel->reg[STATUS] |= 0x10;
    else
        this->regModel->reg[STATUS] &= 0xBF;

}

void PIC::RP0Bit (bool set)
{
    if(set)
        this->regModel->reg[STATUS] |= 0x20;
    else
        this->regModel->reg[STATUS] &= 0xCF;
}
