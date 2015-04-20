#include "pic.h"
#include "QDebug"
#include "math.h"

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



void PIC::ADDWF(){
    qDebug() << "ADDWF";
    /*if(d=0){
    W = W + f;
    }else{
    f = W + f;
    }*/


int erg= regModel->reg[bank][W]+regModel->reg[bank][f];

if(erg > 255)
{
    CBit(true);
    erg=0;
}
else CBit(false);

if(regModel->reg[bank][W]<16 && erg>16)
{
   DCBit(true);
}
else DCBit(false);

if(d==0)
{
    regModel->reg[bank][W]=erg;
}
else regModel->reg[bank][f]=erg;

PC();

}

void PIC::ANDWF(){
    qDebug() << "ANDWF";

    /*if(d=0){
    W = W & f;
    }else{
    f = W & f;
    }*/

    PC();
}

void PIC::CLRF(){
    qDebug() << "CLRF";

    //f  = 0x0;
    PIC::ZBit(true);
    PC();
}

void PIC::CLRW(){
    qDebug() << "CLRW";

    //W = 0x0;
    PIC::ZBit(true);
    PIC::PC();
}

void PIC::COMF(){
    qDebug() << "COMF";

    /*if(d==0){
    W = ~f;}
    else if(d==1){
    f = ~f;}*/
    PIC::PC();
}

void PIC::DECF(){
    qDebug() << "DECF";

   /* if(d==0){
    W = f-1;}
    else if(d==1){
    f = f-1;}*/
    PIC::PC();
}

void PIC::DECFSZ(){
    qDebug() << "DECFSZ";

  /*if(d=1){
   f=f-1;
   if(f!=0){
   PC();}else{NOP();}
   }
   else if(d=0){
   W=f-1;
   if(W!=0){
   PC();}else{NOP();}
   }*/


}

void PIC::INCF(){
    qDebug() << "INCF";

    /*if(d==1){
    f = f+1;}
    else if(d==0){
    W = f+1;}*/
    PC();
}

void PIC::INCFSZ(){
    qDebug() << "INCFSZ";

    /*if(d=1){
    f=f+1;
    if(f!=0){
    PC();}else{NOP();}
    }
    else if(d=0){
    W=f+1;
    if(W!=0){
    PC();}else{NOP();}
    }*/
}

void PIC::IORWF(){
    qDebug() << "IORWF";

    /*if(d==1){
    W = W | f;}
    else if(d==0){
    f = W | f;
    }*/
    PIC::PC();
}

void PIC::MOVF(){
    qDebug() << "MOVF";

    /*if(d==0){
    W = f;}
    else if(d==1){
    f = f;} */
    PIC::PC();
}

void PIC::MOVWF(){
    qDebug() << "MOVWF";

  /*f = W;
    w = 0x0;*/
    //pc++;
}

void PIC::NOP(){
    qDebug() << "NOP";

    PIC::PC();

}

void PIC::RLF(){
    qDebug() << "RLF";
    //Multiplizieren
    /*int carryset = f & 0x80;
    int carryget = this->regModel->reg[bank][STATUS]&0x1;
    f = f*2;
    if(carrget==1){
    f=f+1;
    }else if(carryget==0){
        f=f;
    }
    if(carryset==1){
    PIC::CBit(true);}
    else if(carryset==0){
    PIC::CBit(false);}
    PIC::PC();*/

}

void PIC::RRF(){
    qDebug() << "RRF";
    /*int carryset = f & 0x1;
    int carryget = this->regModel->reg[bank][STATUS]&0x1;
    f = f/2;
    if(carrget==1){
    f=f+128;
    }else if(carryget==0){
        f=f;
    }
    if(carryset==1){
    PIC::CBit(true);}
    else if(carryset==0){
    PIC::CBit(false);}
    PIC::PC();*/

}

void PIC::SUBWF(){
    qDebug() << "SUBWF";

    /*if(d==0){
        W = f - W;}else if(d==1){
        f = f -W;}*/

    PIC::PC();}

void PIC::SWAPF(){
    qDebug() << "SWAPF";
    /*int leftnibble = f & 0xF0;
    int rightnibble = f & 0xF;
    leftnibble = leftnibble / 16;
    rightnibble = rightnibble * 16;
    f = f | leftnibble;
    f = f | rightnibble;*/
    PIC::PC();

}

void PIC::XORWF(){
    qDebug() << "XORWF";

    /*if(d==0){
    W = W ^ f;}
    else if(d==1){
    f = W ^ f;}*/

    PIC::PC();
}

void PIC::BCF(){
    qDebug() << "BCF";

    //Verunden mit dem 1er-Complement von 2^b
    //f = f & ~pow(2,b);
    //pc++;

}


void PIC::BSF(){
    qDebug() << "BSF";

    //Verodern mit 2^b
    //f= f | pow(2,b);
    //pc++;
}

void PIC::BTFSC(){
    qDebug() << "BTFSC";
    /*if(b=0){
        pc++;
    }else{NOP();}*/
    //pc++;

}

void PIC::BTFSS(){
    qDebug() << "BTFSS";
    /*if(b=1){
        pc++;
    }else{NOP();}*/


}

void PIC::ADDLW(){
    qDebug() << "ADDLW";

  //W = l + W;
    //pc++;
}

void PIC::ANDLW(){
    qDebug() << "ANDLW";

    //W = l & W;
    //pc++;
}

void PIC::CALL(){
    qDebug() << "CALL";
    PIC::PC();


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

void PIC::MOVLW(){
    qDebug() << "MOVLW1";

    //W = l;
    //pc++;
}

void PIC::IORLW(){
    qDebug() << "IORLW";

    //W = l | W;
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
}
