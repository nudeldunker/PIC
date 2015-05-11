#include "pic.h"
#include "QDebug"
#include "math.h"


PIC::PIC(QObject *parent) : QObject(parent)
{
//    regModel=new RegModel(parent);
//    regModelDlgt= new RegModelDlgt(qobject_cast<QWidget*>(parent));

}

void PIC::init()
{
    regModel=new RegModel();
    regModelDlgt=new RegModelDlgt();
    emit pointer();

}

PIC::~PIC()
{

}

void PIC::runCode()
{
    int pc=getPC();
    while( pc< m_CmdList.size() && (!stop || singleStep))
    {
        decodeCmd(pc);
        pc=getPC();

        if(singleStep==true) qDebug()<<"---------------------------------------------------------------------------Single Step";

        singleStep=false;
    }
}

void PIC::stopExec(bool stop)
{
    stop=stop;
    qDebug()<< stop;
}

int PIC::getPC()
{
  return (regModel->reg[bank][PCLATH]<<8) +regModel->reg[bank][PCL];
}

void PIC::decodeCmd(int pc)
{
//    {

        //qDebug() << k_long << "klong";
        qDebug() << pc << "PC";

        k_long=m_CmdList[pc] & 0x7FF;
        //qDebug() << "CMDLIST" <<m_CmdList[pc];
        k=m_CmdList[pc] & 0xFF;
        //qDebug() << k << "k";
        f=m_CmdList[pc] & 0x7F;
        //qDebug() << f <<"f";
        d=m_CmdList[pc] & 0x80;
        d=(d>>7); //Test
        l=d;
        b=m_CmdList[pc] & 0x380;
        //qDebug() << b << "b";

        SetBank();
        ChkIndirect();

     int ByteCmd=m_CmdList[pc] & 0x3F00;
     //qDebug() << ByteCmd << "byteCMD";
     int BitCmd=m_CmdList[pc] & 0x3C00;
     //qDebug() << BitCmd << "BitCMD";
     int ShrtCmd=m_CmdList[pc] & 0x3800;
     //qDebug() << ShrtCmd << "ShrtCMD";

     if(ByteCmd == 0x0700 )
        ADDWF();
     else if(ByteCmd == 0x0500)
        ANDWF();
     else if((m_CmdList[pc] & 0x03F80)  == 0x0180)
        CLRF();
     else if(ByteCmd == 0x0100)
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
     //ADDLW kann durch don't care Bit 3E bzw. 3F sein
     else if((m_CmdList[pc] & 0x3E00 ) == 0x3E00)
        ADDLW();
     else if((m_CmdList[pc] & 0x3F00 ) == 0x3F00)
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
        SUBLW();
     else if((m_CmdList[pc] & 0xFFFF ) == 0x0063)
        SLEEP();
    else if((m_CmdList[pc] & 0xFFFF ) == 0x0008)
        RETURN();
    else if((BitCmd) == 0x3400)
        RETURNLW();
    else if((m_CmdList[pc] & 0xFFFF ) == 0x0009)
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



     //Diagnoseausgaben
     qDebug() << "---------------------------------";
//     qDebug() << regModel->reg[bank][PORTB] << "PortB";
     qDebug() << regModel->reg[bank][INDIRECT] << "INDIRECT";
     qDebug() << regModel->reg[bank][FSR] << "FSR";
//     qDebug() << regModel->reg[bank][0x15] << "15h";
     qDebug() << cycles << "Programmzyklen";

     qDebug() << "---------------------------------";

}

void PIC::ChkIndirect(){
    if(f == 0x0){
        f = regModel->reg[0][FSR];
        qDebug() << "Indirect Bank-0 / f" << f;
    }
    if(f == 0x80){
        f = regModel->reg[1][FSR];
        qDebug() << "Indirect Bank-1 / f" << f;
    }

}

void PIC::ADDWF(){
    qDebug() << "ADDWF";


if(f == 0x03 | f == 0x83){
    qDebug() << "Ziel ist Status Register";
    qDebug() << W << " = W";
    erg = W && 0xE0;
    qDebug() << "W nach der Operation" << W;
    qDebug() << regModel->reg[bank][f] << "Wert in f";
    erg = erg + regModel->reg[bank][f];
    qDebug() << "Wert in f nach Operation" << regModel->reg[bank][f];

}else{
    qDebug() << "W vorher" << W;
    qDebug() << "Inhalt von f vorher" << regModel->reg[bank][f];
    erg = W+regModel->reg[bank][f];
    qDebug() << "Ergebnis" << erg;
}

if(erg >=256)
{
    qDebug() << "ergebnis war > 255";
    erg =  erg - 256;
    qDebug() << "ergebnis" << erg;
}
ChkZBit(erg);
ChkCBit(erg);
ChkDCBit(erg);
qDebug() << d << "D";
if(d==0)
{
    W=erg;
}
else regModel->reg[bank][f]=erg;

PC();

}

void PIC::ANDWF(){
    qDebug() << "ANDWF";

    int erg= W && regModel->reg[bank][f];
    if(erg >=256)
    {
        erg =  erg - 256;
    }
    ChkCBit(erg);

    ChkZBit(erg);

    if(d==0){
    W=erg;
    }else{
    regModel->reg[bank][f]=erg;
    }

    PC();


}

void PIC::CLRF(){
    qDebug() << "CLRF";
    qDebug() << f;
    //status Register -> nur die obersten 3 Bit werden gecleared, der Rest bleibt unchanged.
    if(f == 0x03 | f == 0x83){
    regModel->reg[bank][f]=regModel->reg[bank][f] && 0x1F;
    }else{
    regModel->reg[bank][f]=0;}
    ZBit(true);
    PC();

    }

void PIC::CLRW(){
    qDebug() << "CLRW";

    qDebug() << "W-vorher" << W;
    W=0;
    qDebug() << "W-später" << W;
    ZBit(true);
    PC();
}

void PIC::COMF(){
    qDebug() << "COMF";

    erg = regModel->reg[bank][f];
    ChkZBit(erg);
    qDebug() << erg;
    erg = erg ^ 0xff;
    qDebug() << erg;

    if(erg >=256){
        erg =  erg - 256;
    }

    ChkZBit(erg);

    if(d==0){
    W=erg;}
    else if(d==1){
    regModel->reg[bank][f]=erg;}
    PC();
}

void PIC::DECF(){
    qDebug() << "DECF";

    erg = regModel->reg[bank][f];
    qDebug() << erg;
    erg = erg-1;
    qDebug() << erg;

    if(erg < 0){
        erg = 256 + erg;
    }

    ChkZBit(erg);

    if(d==0){
    W= erg;}
    else if(d==1){
    regModel->reg[bank][f]=erg;}


    PC();
}

void PIC::DECFSZ(){
    qDebug() << "DECFSZ";

    erg = regModel->reg[bank][f];
    qDebug() << erg;
    erg = erg-1;
    qDebug() << erg;

    if(erg < 0){
        erg = 256 + erg;
    }

    ChkZBit(erg);


    if(d==1){
        regModel->reg[bank][f]=erg;
        } else if(d==0){
        W=erg;
        }
    if(erg == 0){
        cycles++;
        PC();
        NOP();
    } else if(erg > 0){
        PC();
    }


}

void PIC::INCF(){
    qDebug() << "INCF";

    erg = regModel->reg[bank][f];
    qDebug() << erg;
    erg = erg+1;
    qDebug() << erg;

    if(erg == 256){
        erg = 0;}

    ChkZBit(erg);

    if(d==0){
    W= erg;}
    else if(d==1){
    regModel->reg[bank][f]=erg;}


    PC();


}

void PIC::INCFSZ(){
    qDebug() << "INCFSZ";

    erg = regModel->reg[bank][f];
    qDebug() << erg;
    erg = erg+1;
    qDebug() << erg;
    if(erg == 256){
        erg = 0;}

    if(d==1){
     regModel->reg[bank][f]=erg;
     if(regModel->reg[bank][f]==0){
     PC();}else{
         cycles++;
         PC();
         NOP();}
     }
     else if(d==0){
     W=erg;
     if(W==0){
     PC();}
     else{
     cycles++;
     PC();
     NOP();}
     }

}

void PIC::IORWF(){
    qDebug() << "IORWF";

    qDebug() << regModel->reg[bank][f];
    qDebug() << W;
    erg=regModel->reg[bank][f]||W;
    qDebug() << erg;

    if(erg >=256){
        erg =  erg - 256;
    }
    ChkZBit(erg);

    if(d==1){
    W = erg;}
    else if(d==0){
    regModel->reg[bank][f] = erg;
    }

    PC();
}

void PIC::MOVF(){
    qDebug() << "MOVF";

    qDebug() << f << "f";
    qDebug() << regModel->reg[bank][f] << "inhalt f";

    if(d==0){
    W = regModel->reg[bank][f];}
    else if(d==1){
    regModel->reg[bank][f] = regModel->reg[bank][f];
    ZBit(true);}

    qDebug() << W;


    PC();
}

void PIC::MOVWF(){
    qDebug() << "MOVWF";

  /*f = W;
    w = 0x0;*/
    qDebug() << "f="<< f;
    qDebug() << "W="<< W;
    regModel->reg[bank][f] = W;
    //f = W;
    qDebug() << "Zelle: " << f << "Inhalt: "<< regModel->reg[bank][f];
    PC();
}

void PIC::NOP(){
    qDebug() << "NOP";
    qDebug() << regModel->reg[bank][PCL];
    PC();
    qDebug() << regModel->reg[bank][PCL];
}

void PIC::RLF(){
    qDebug() << "RLF";
    //Multiplizieren

    int carryset = regModel->reg[bank][f] & 0x80;
    int carryget = regModel->reg[bank][STATUS]&0x1;
    qDebug() << regModel->reg[bank][f] << "f";
    regModel->reg[bank][f] = regModel->reg[bank][f]*2;
    qDebug() << regModel->reg[bank][f] << "f";
    if(carryget==1){
    regModel->reg[bank][f]=regModel->reg[bank][f]+1;
    }else if(carryget==0){
        regModel->reg[bank][f]=regModel->reg[bank][f];
    }
    if(carryset==1){
    CBit(true);}
    else if(carryset==0){
    CBit(false);}
    PC();


}

void PIC::RRF(){
    qDebug() << "RRF";

    int carryset = regModel->reg[bank][f] & 0x80;
    int carryget = regModel->reg[bank][STATUS]&0x1;

    regModel->reg[bank][f] = regModel->reg[bank][f]/2;
    if(carryget==1){
    regModel->reg[bank][f]=regModel->reg[bank][f]+128;
    }else if(carryget==0){
        regModel->reg[bank][f]=regModel->reg[bank][f];
    }
    if(carryset==1){
    CBit(true);}
    else if(carryset==0){
    CBit(false);}
    PC();

}

void PIC::SUBWF(){
    qDebug() << "SUBWF";

    qDebug() << regModel->reg[bank][f];
    qDebug() << W;

    erg = regModel->reg[bank][f] - W;

    if(erg < 0){
        erg = 256 + erg;
        CBit(true);
    }else{CBit(false);}


    ChkDCBit(erg);
    ChkZBit(erg);

    if(d==0){
        W = erg;
        qDebug() << W;
    }
    else if(d==1){
        regModel->reg[bank][f] = erg;
        qDebug() << regModel->reg[bank][f];
    }

    PC();
}

void PIC::SWAPF(){
    qDebug() << "SWAPF";
    /*int leftnibble = f & 0xF0;
    int rightnibble = f & 0xF;
    leftnibble = leftnibble / 16;
    rightnibble = rightnibble * 16;
    f = f | leftnibble;
    f = f | rightnibble;*/

    int leftnibble = regModel->reg[bank][f] & 0xF0;
    int rightnibble = regModel->reg[bank][f] & 0xF;
    leftnibble = leftnibble / 16;
    rightnibble = rightnibble * 16;
    regModel->reg[bank][f] = regModel->reg[bank][f] || leftnibble;
    regModel->reg[bank][f] = regModel->reg[bank][f] || rightnibble;

    PC();

}

void PIC::XORWF(){
    qDebug() << "XORWF";

    erg = W^regModel->reg[bank][f];

    ChkZBit(erg);

    if(d==0){
    W = erg;}
    else if(d==1){
    regModel->reg[bank][f] = erg;}

    PC();
}

void PIC::BCF(){
    qDebug() << "BCF";

    //Verunden mit dem 1er-Complement von 2^b
    //f = f & ~pow(2,b);
    erg = pow(2,b);
    erg = erg ^ 0xFF;
    regModel->reg[bank][f] = regModel->reg[bank][f] && erg;
    PC();

}


void PIC::BSF(){
    qDebug() << "BSF";

    //Verodern mit 2^b
    //f= f | pow(2,b);
    regModel->reg[bank][f]= regModel->reg[bank][f] || pow(2,b);
    PC();
}

void PIC::BTFSC(){
    qDebug() << "BTFSC";
    if(b==0){
        cycles++;
        PC();
        NOP();
    }else{
    PC();}


}

void PIC::BTFSS(){
    qDebug() << "BTFSS";
    if(b==1){
        cycles++;
        PC();
        NOP();
    }else{PC();}


}

void PIC::ADDLW(){
    qDebug() << "ADDLW";

    //int erg = regModel->reg[bank][W] + k;
    qDebug() << W <<"W";
    qDebug() << k <<"k";
    erg = W + k;
    if(erg >=256){
        erg =  erg - 256;
        CBit(true);

    }else{CBit(false);}
    qDebug() << erg << "erg";
    W=erg;
    ChkCBit(erg);
    ChkDCBit(erg);
    ChkZBit(erg);
    PC();
}

void PIC::ANDLW(){
    qDebug() << "ANDLW";

    erg = W && k;

    ChkZBit(erg);

    W=erg;

    PC();
}

void PIC::CALL(){
    qDebug() << "CALL";
    cycles++;
    PIC::pushstack();
}

void PIC::CLRWDT(){
    qDebug() << "CLRWDT";

    //wdt = 0x0;
    PC();
}

void PIC::GOTO(){
    qDebug() << "GOTO";
    qDebug() << "PCL-vorher" << regModel->reg[bank][PCL];
    cycles++;
    regModel->reg[bank][PCL] = k;
    qDebug() << "PCL-nacher"<< regModel->reg[bank][PCL];

}

void PIC::XORLW(){
    qDebug() << "XORLW";

    erg = W ^ k;
    if(erg >=256){
        erg =  erg - 256;
        CBit(true);
    }else{CBit(false);}
    W=erg;

    ChkZBit(erg);
    PC();
    }

void PIC::SUBLW(){
    qDebug() << "SUBLW1";

    erg =k - W;
    if(erg < 0){
        erg = 256 + erg;
        CBit(true);
    }else{CBit(false);}
    W=erg;
    ChkZBit(erg);
    PC();
}

void PIC::SLEEP(){
    qDebug() << "SLEEP";

    //system("pause");
    PC();
}

void PIC::RETURN(){
    qDebug() << "RETURN";
    qDebug() << "pcl" << regModel->reg[bank][PCL];
    cycles++;
    PIC::popstack();
    qDebug() << "pcl" << regModel->reg[bank][PCL];

}

void PIC::RETURNLW(){
    qDebug() << "RETURNLW";
    cycles++;
    W = k;
    RETURN();


}

void PIC::RETURNFIE(){
    qDebug() << "RETURNFIE";
    cycles++;
    PC();

}

void PIC::MOVLW(){
    qDebug() << "MOVLW";
    qDebug() << "k" << k;
    W=k;
    qDebug() << "W" << W;
    //don't cares = 0!
    PC();
}

void PIC::IORLW(){
    qDebug() << "IORLW";

    erg=W || k;
    W = erg;
    ChkZBit(erg);
    PC();
}

void PIC::SetBank(){

    int helper = regModel->reg[bank][STATUS];
    helper = helper && 0x20;
    if(helper == 0x0){
    bank = 0;}
    else if(helper == 0x20){
        bank = 1;
    }

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
    PIC::cycles++;
}


int PIC::ChkCBit(int){

    if(erg >=256)
    {
        CBit(true);
        return erg=0;
    }
    else CBit(false);

}

void PIC::ChkDCBit(int){

    if(regModel->reg[bank][W]<16 && erg>16)
    {
       DCBit(true);
    }
    else DCBit(false);
}

void PIC::updateReg(){
    regModel->dataChanged(regModel->index(0,0,QModelIndex()), regModel->index(regModel->rowCount()-1, regModel->columnCount()-1,QModelIndex()));
}

void PIC::finish()
{
    emit finished();
}

void PIC::ChkZBit(int){


        if(erg==0)
            ZBit(true);
        else ZBit(false);

}

void PIC::pushstack(){
    int pctemp = regModel->reg[bank][PCL];
    qDebug() << regModel->reg[bank][PCL] << "PCL";
    qDebug() << pctemp << "pctemp";
    pctemp = pctemp + 1;
    qDebug() << pctemp << "pctemp";
    qDebug() << stackpointer << "stackpointer";
    if(stackpointer == 7){
        stackpointer = 0;
    qDebug() << stackpointer << "stackpointer";
    }
    qDebug() << stack[stackpointer] << "stackinhalt";
    stack[stackpointer] = pctemp;
    qDebug() << stack[stackpointer] << "stackinhalt";
    qDebug() << stackpointer << "stackpointer";
    stackpointer++;
    qDebug() << stackpointer << "stackpointer";
    regModel->reg[bank][PCL] = k;
    qDebug() << "_________________________________";
}

void PIC::popstack(){
    qDebug() << stackpointer << "stackpointer";
    if(stackpointer == 0){
        stackpointer = 7;
        qDebug() << stackpointer << "stackpointer";
    }
    qDebug() << stack[stackpointer] << "stackinhalt";
    stackpointer --;
    regModel->reg[bank][PCL] = stack[stackpointer];
    //stackpointer --;
    qDebug() << stackpointer << "stackpointer";
}

//PSA - PreScaler Beziehungen
void PIC::SetPSA(){
    //Prescaler hängt am Watchdog
    //TMR0 Verhältnis = 1:1
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x4;
}

void PIC::ClearPSA(){
    //Prescaler hängt an TMR0
    int einercomp = 0x4;
    einercomp = ~ einercomp;
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] & einercomp;
}





//Maskierung des PreScaler Verhältnisses
void PIC::SetPS000(){
    //TMR0/1:2 WD/1:1
    int einercomp = 0x7;
    einercomp = ~ einercomp;
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] & einercomp;
}

void PIC::SetPS001(){
    //TMR0/1:4 WD/1:2
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x1;
}

void PIC::SetPS010(){
    //TMR0/1:8 WD/1:4
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x2;
}

void PIC::SetPS011(){

    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x3;
}

void PIC::SetPS100(){
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x4;
}

void PIC::SetPS101(){
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x5;
}

void PIC::SetPS110(){
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x6;
}

void PIC::SetPS111(){
    //TMR0/1:256 WD/1:128
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] | 0x7;
}

