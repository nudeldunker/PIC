#include "pic.h"
#include "QDebug"

PIC::PIC(QObject *parent) : QObject(parent)
{
//    regModel=new RegModel(parent);
//    regModelDlgt= new RegModelDlgt(qobject_cast<QWidget*>(parent));

}

void PIC::init()
{
    regModel=new RegModel();
    regModelDlgt=new RegModelDlgt();
    emit pointer(regModel,regModelDlgt);

}

PIC::~PIC()
{

}

void PIC::runCode()
{
    int pc=getPC();
    while( pc< m_CmdList.size() && !stop)
    {
        decodeCmd(pc);
        pc=getPC();
    }
}

void PIC::stopExec(bool stop)
{
    stop=stop;
}

int PIC::getPC()
{
  return (regModel->reg[bank][PCLATH]<<8) +regModel->reg[bank][PCL];
}

void PIC::decodeCmd(int pc)
{


//    for( int i=0; i<m_CmdList.size(); i++)
//    {
        k_long=m_CmdList[pc] & 0x7FF;
        k=m_CmdList[pc] & 0xFF;
        f=m_CmdList[pc] & 0x7F;
        d=m_CmdList[pc] & 0x80;
        l=d;
        b=m_CmdList[pc] & 0x380;

     int ByteCmd=m_CmdList[pc] & 0x3F00;
     int BitCmd=m_CmdList[pc] & 0x3C00;
     int ShrtCmd=m_CmdList[pc] & 0x3800;

     if(ByteCmd == 0x0700 )
        ADDWF();
     else if(ByteCmd == 0x0500)
        ANDWF();
     else if(ByteCmd == 0x0100)
        CLRF();
     else if((m_CmdList[pc] & 0x3F80) == 0x0100)
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
     else if((m_CmdList[pc] & 0x3F9F) == 0x0000)
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
     else if((m_CmdList[pc] & 0x3E00 ) == 0x3E00)
        ADDLW();
     else if(ByteCmd == 0x3900)
        ANDLW();
     else if(ShrtCmd == 0x2000)
        CALL();
     else if((m_CmdList[pc]& 0XFFFF) == 0x0064)
        CLRWDT();
     else if(ShrtCmd == 0x2800)
        GOTO();
     else if((ByteCmd) == 0x3A00)
        XORLW();
     else if((m_CmdList[pc] & 0x3E00 ) == 0x3C00)
        SUBLW1();
     else if((ByteCmd) == 0x3C00)
        SUBLW2();
    else if((m_CmdList[pc] & 0xFFFF ) == 0x0063)
        SLEEP();
    else if((m_CmdList[pc] & 0xFFFF ) == 0x0008)
        RETURN();
    else if((BitCmd) == 0x3400)
        RETURNLW();
    else if((m_CmdList[pc] & 0xFFFF ) == 0x0009)
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
//}
}


void PIC::ADDWF(){
    qDebug() << "ADDWF";
    /*if(d=0){
    W = W + f;
    }else{
    f = W + f;
    }*/

    int erg= W +regModel->reg[bank][f];

    if(erg==0)
        ZBit(true);
    else ZBit(false);

    if(erg > 255)
    {
        CBit(true);
        erg=0;
    }
    else CBit(false);

    if(W<16 && erg>16)
    {
       DCBit(true);
    }
    else DCBit(false);

    if(d==0)
    {
        W=erg;
    }
    else regModel->reg[bank][f]=erg;

    PC();

    //W = W + f;
    //pc++;


}

void PIC::ANDWF(){
    qDebug() << "ANDWF";

    /*if(d=0){
    W = W & f;
    }else{
    f = W & f;
    }

    pc++; */
    int erg= W & regModel->reg[bank][f];
    if(erg==0)
    {
        ZBit(true);
    }
    else ZBit(false);

    if(d==0)
    {
        W=erg;
    }
    else regModel->reg[bank][f]=erg;


    PC();


}

void PIC::CLRF(){
    qDebug() << "CLRF";
    regModel->reg[bank][f]=0;
    ZBit(true);
    PC();
    //f  = 0x0;
    //pc++;
}

void PIC::CLRW(){
    qDebug() << "CLRW";
    W=0;
    ZBit(true);
    PC();
    //W = 0x0;
    //pc++;
}

void PIC::COMF(){
    qDebug() << "COMF";

    int erg=~regModel->reg[bank][f];

    if(erg==0)
        ZBit(true);
    else ZBit(false);

    if(d==0)
    {
        W= erg;
    }
    else regModel->reg[bank][f]=erg;

    //f = ~f;
    //pc++;
}

void PIC::DECF(){
    qDebug() << "DECF";

    //f = f-1;
    //pc++;
}

void PIC::DECFSZ(){
    qDebug() << "DECFSZ";

    //Decrement f skip if 0
   /* if(f != 0x0){
       f = f-1;
    } else{
        f=f;
    }*/
//pc++;
}

void PIC::INCF(){
    qDebug() << "INCF";

    //f = f+1;
    //pc++;
}

void PIC::INCFSZ(){
    qDebug() << "INCFSZ";

    /* if(f != 0x0){
        f = f+1;
     } else{
         f=f;
     }*/
    //pc++;
}

void PIC::IORWF(){
    qDebug() << "IORWF";

    //W = W | f;
    //pc++;
}

void PIC::MOVF(){
    qDebug() << "MOVF";

    /*x = f;
    f = 0x0;*/
    //pc++;
}

void PIC::MOVWF(){
    qDebug() << "MOVWF";

  /*f = W;
    w = 0x0;*/
    //pc++;
}

void PIC::NOP(){
    qDebug() << "NOP";

    //pc++;

}

void PIC::RLF(){
    qDebug() << "RLF";

    //pc++;

}

void PIC::RRF(){
    qDebug() << "RRF";

    //pc++;

}

void PIC::SUBWF(){
    qDebug() << "SUBWF";

  //f = f-W;
    //pc++;
}

void PIC::SWAPF(){
    qDebug() << "SWAPF";

    //swap nibbles? watt?
    //pc++;
}

void PIC::XORWF(){
    qDebug() << "XORWF";

    //W = W ^ f;
    //pc++;
}

void PIC::BCF(){
    qDebug() << "BCF";


    //pc++;

}


void PIC::BSF(){
    qDebug() << "BSF";

    //pc++;

}

void PIC::BTFSC(){
    qDebug() << "BTFSC";

    //pc++;

}

void PIC::BTFSS(){
    qDebug() << "BTFSS";

    //pc++;

}

void PIC::ADDLW(){
    qDebug() << "ADDLW";

  //w = l + W;
    //pc++;
}

void PIC::ANDLW(){
    qDebug() << "ANDLW";

    //w = l & W;
    //pc++;
}

void PIC::CALL(){
    qDebug() << "CALL";
    //pc++;


}

void PIC::CLRWDT(){
    qDebug() << "CLRWDT";

    //wdt = 0x0;
    //pc++;
}

void PIC::GOTO(){
    qDebug() << "GOTO";

    //pc++;

}

void PIC::XORLW(){
    qDebug() << "XORLW";

    //l = l ^ W;
    //pc++;
}

void PIC::SUBLW1(){
    qDebug() << "SUBLW1";

    // l = l-W;
    //pc++;
}

void PIC::SUBLW2(){
    qDebug() << "SUBLW2";

    // l = l-W;
    //pc++;
}

void PIC::SLEEP(){
    qDebug() << "SLEEP";

    system("pause");
    //pc++;
}

void PIC::RETURN(){
    qDebug() << "RETURN";

    //pc++;

}

void PIC::RETURNLW(){
    qDebug() << "RETURNLW";

    //pc++;

}

void PIC::RETURNFIE(){
    qDebug() << "RETURNFIE";

    //pc++;

}

void PIC::MOVLW1(){
    qDebug() << "MOVLW1";

    //W = l;
    //pc++;
    PC();
}

void PIC::MOVLW2(){
    qDebug() << "MOVLW2";

    //W = l;
    //pc++;
}

void PIC::MOVLW3(){
    qDebug() << "MOVLW3";

    //W = l;
    //pc++;
}

void PIC::MOVLW4(){
    qDebug() << "MOVLW4";

    //W = l;
    //pc++;
}

void PIC::IORLW(){
    qDebug() << "IORLW";

    //l = l | W;
    //pc++;

}



void PIC::CBit(bool set)
{
    if(set)
        this->regModel->reg[bank][STATUS] =regModel->reg[bank][STATUS] | 0x1;
    else

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] & 0xFE;
}

void PIC::DCBit(bool set)
{
    if(set)

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] | 0x2;
    else

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] & 0xFD;
}

void PIC::ZBit(bool set)
{
    if(set)

        this->regModel->reg[bank][STATUS] =regModel->reg[bank][STATUS] | 0x4;
    else

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] & 0xFB;

}

void PIC::PDBit (bool set)
{
    if(set)

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] | 0x8;
    else

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] & 0xF7;

}

void PIC::TOBit (bool set)
{
    if(set)

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] | 0x10;
    else

        this->regModel->reg[bank][STATUS] = regModel->reg[bank][STATUS] & 0xBF;

}

void PIC::RP0Bit (bool set)
{
    if(set)

        this->regModel->reg[bank][STATUS]= regModel->reg[bank][STATUS] | 0x20;
    else

        this->regModel->reg[bank][STATUS]= regModel->reg[bank][STATUS] & 0xCF;
}

void PIC::PC()
{
    regModel->reg[bank][PCL]++;
    if(regModel->reg[bank][PCL]>255)
    {
        regModel->reg[bank][PCL]&= 0xFF;
        regModel->reg[bank][PCLATH]++;
    }

    regModel->dataChanged(regModel->index(0,0, QModelIndex()), regModel->index(0,0, QModelIndex()));
}

void PIC::updateReg()
{
    regModel->dataChanged(regModel->index(0,0,QModelIndex()), regModel->index(regModel->rowCount()-1, regModel->columnCount()-1,QModelIndex()));
}
