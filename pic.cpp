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

        qDebug() << "CMDLIST" <<m_CmdList[pc];
        qDebug() << pc << "PC";
        k_long=m_CmdList[pc] & 0x7FF;
        qDebug() << k_long << "klong";
        k=m_CmdList[pc] & 0xFF;
        qDebug() << k << "k";
        f=m_CmdList[pc] & 0x7F;
        qDebug() << f <<"f";
        d=m_CmdList[pc] & 0x80;
        d=(d>>7); //Test
        l=d;
        b=m_CmdList[pc] & 0x380;
        b = b / 128;
        qDebug() << b << "b";
        qDebug() << PreScalerWert << "PreScalerWert";

        PIC::getPreScaler();
        PIC::SetBank();
        PIC::ChkIndirect();
        PIC::setTmr0();


     int ByteCmd=m_CmdList[pc] & 0x3F00;
     qDebug() << ByteCmd << "byteCMD";
     int BitCmd=m_CmdList[pc] & 0x3C00;
     qDebug() << BitCmd << "BitCMD";
     int ShrtCmd=m_CmdList[pc] & 0x3800;
     qDebug() << ShrtCmd << "ShrtCMD";


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
     else if((m_CmdList[pc] & 0x3F80) == 0x0080)
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

    //zählt nach jeder Befehlsabarbeitung einen Programmzyklus hoch, bizyklische Befehle zählen zusätzlich während des Befehls rauf
     PIC::ExtClock();
     PIC::IncrementCycles();
     PIC::LaufZeit();
     PreScalerCounter++;
     PIC::SyncSpecialReg();
     PIC::RBPeakAnalyzer();
     PIC::InterruptAnalyzer();


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
    erg = W & 0xE0;
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
    qDebug() << "W vor der Operation: " << W;
    qDebug() << "Inhalt der Zelle " << regModel->reg[bank][f] << "an der Stelle " << f;
    erg = W & regModel->reg[bank][f];
    qDebug() << "Ergebnis der Operation" << erg;
    if(erg >= 256)
    {
        erg =  erg - 256;
    }
    ChkCBit(erg);

    ChkZBit(erg);

    if(d==0){
    qDebug() << "das Ergebnis wird in W geschrieben";
    W=erg;
    }else{
    qDebug() << "das Ergebnis wird in f geschrieben";
    regModel->reg[bank][f]=erg;
    }

    PC();


}

void PIC::CLRF(){
    qDebug() << "CLRF";
    qDebug() << f;
    //status Register -> nur die obersten 3 Bit werden gecleared, der Rest bleibt unchanged.
    if(f == 0x03 | f == 0x83){
    regModel->reg[bank][f]=regModel->reg[bank][f] & 0x1F;
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
        IncrementCycles();
        PreScalerCounter++;
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
         IncrementCycles();
         PreScalerCounter++;
         PC();
         NOP();}
     }
     else if(d==0){
     W=erg;
     if(W==0){
     PC();}
     else{
     IncrementCycles();
     PreScalerCounter++;
     PC();
     NOP();}
     }

}

void PIC::IORWF(){
    qDebug() << "IORWF";

    qDebug() << regModel->reg[bank][f];
    qDebug() << W;
    erg=regModel->reg[bank][f]|W;
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
    int erg = regModel->reg[bank][f];
    if(d==0){
    W = erg;}
    else if(d==1){
    regModel->reg[bank][f] = erg;
    }

//    qDebug() << W;
    ChkZBit(erg);

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
    regModel->reg[bank][f] = regModel->reg[bank][f] | leftnibble;
    regModel->reg[bank][f] = regModel->reg[bank][f] | rightnibble;

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
    qDebug() << erg << "erg pow(2,b)";
    erg = erg ^ 0xFF;
    qDebug() << regModel->reg[bank][f] << "inhalt f vorher";
    regModel->reg[bank][f] = regModel->reg[bank][f] & erg;
    qDebug() << regModel->reg[bank][f] << "inhalt f später";
    PC();

}


void PIC::BSF(){
    qDebug() << "BSF";

    //Verodern mit 2^b
    //f= f | pow(2,b);
    int reginhalt = regModel->reg[bank][f];
    int temp = pow(2,b);
    qDebug() << temp << "temp";
    temp = reginhalt | temp;
    regModel->reg[bank][f]= temp;
    PC();
}

void PIC::BTFSC(){
    qDebug() << "BTFSC";
    int ftemp = regModel->reg[bank][f];
    int referenz = pow(2,b);
    referenz = referenz ^ 0xFF;
    ftemp = ftemp | referenz;
    if(ftemp == referenz){
        IncrementCycles();
        PreScalerCounter++;
        PC();
        NOP();
    }else{PC();}

    }

void PIC::BTFSS(){
    qDebug() << "BTFSS";
    int ftemp = regModel->reg[bank][f];
    int referenz = pow(2,b);
    ftemp = ftemp & referenz;
    if(ftemp == referenz){
        IncrementCycles();
        PreScalerCounter++;
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

    erg = W & k;

    ChkZBit(erg);

    W=erg;

    PC();
}

void PIC::CALL(){
    qDebug() << "CALL";
    IncrementCycles();
    PreScalerCounter++;
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
    IncrementCycles();
    PreScalerCounter++;
    PIC::popstack();
    qDebug() << "pcl" << regModel->reg[bank][PCL];

}

void PIC::RETURNLW(){
    qDebug() << "RETURNLW";
    W = k;
    PIC::RETURN();


}

void PIC::RETURNFIE(){
    qDebug() << "RETURNFIE";
    int intcon = regModel->reg[bank][INTCON];
    intcon = intcon | 0x80;
    regModel->reg[bank][INTCON] = intcon;
    PIC::RETURN();
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

    erg=W | k;
    W = erg;
    ChkZBit(erg);
    PC();
}

void PIC::SetBank(){

    int helper = regModel->reg[bank][STATUS];
    qDebug() << "Status Register Inhalt" << helper;
    helper = helper & 0x20;
    qDebug() << "aktuelle Bank in Status Register" << helper;
    if(helper == 0x0){
    bank = 0;}
    else if(helper == 0x20){
        bank = 1;
    }

    qDebug() << bank << "bank";

}

void PIC::SetRegister(){

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

void PIC::ChkZBit(int){


        if(erg==0)
            ZBit(true);
        else ZBit(false);

}

void PIC::updateReg(){
    regModel->dataChanged(regModel->index(0,0,QModelIndex()), regModel->index(regModel->rowCount()-1, regModel->columnCount()-1,QModelIndex()));
}

void PIC::finish()
{
    emit finished();
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
    einercomp = einercomp ^ 0xFF;
    regModel->reg[1][OPTION] = regModel->reg[1][OPTION] & einercomp;
}

void PIC::getPreScaler(){
    int cleanPS = regModel->reg[1][0x01];
    qDebug() << "cleanPS/OptionReg" << cleanPS << regModel->reg[1][0x01];
    cleanPS = cleanPS & 0x07;
    qDebug() << "cleanPS maskiert/OptionReg" << cleanPS << regModel->reg[1][0x01];
    PreScalerWert = pow(2,cleanPS);
    qDebug() << PreScalerWert << "PreScalerWert";
}

void PIC::setTmr0(){
    int T0CS = regModel->reg[1][OPTION];
    T0CS = T0CS & 0x20;

    //Prescaler Affinität prüfen
    int PSA = regModel->reg[1][OPTION];
    PSA = PSA & 0x08;

    //PreScaler am WDT-> Verhältnis 1:1
    if(0x08 == PSA){
        qDebug() << "PreScalerTmr0 false";
        PreScalerTmr0 = false;
    }
    else{
        qDebug() << "PreScalerTmr0 true";
        PreScalerTmr0 = true;
    }

    if(T0CS == 0x20){
        qDebug() << "Tmr0 Counter";
        Tmr0Counter();
    }else if(T0CS == 0x00){
        qDebug() << "Tmr0 Timer";
        Tmr0Timer();
    }

    qDebug() << regModel->reg[0][TMR0] << "Timer0";
}

void PIC::Tmr0Timer(){

    //PreScaler am WDT-> Verhältnis 1:1
    if(PreScalerTmr0 == false){
    regModel->reg[1][TMR0] = regModel->reg[0][TMR0] ++;
    qDebug() << "Tmr0 Timer 1:1" << regModel->reg[0][TMR0];
    }
    else{
    PIC::Tmr0Increment();
    }
    PIC::Tmr0overflow();

}





void PIC::Tmr0Counter(){
    int AktuelleFlanke = regModel->reg[0][0x05] & 0x10;
    int Tmr0SE = regModel->reg[1][OPTION] & 0x10;

    //1:1
    if(PreScalerTmr0 == false){
    qDebug() << "Tmr0 Counter 1:1";
    //steigende Flanke 0->1

        if(Tmr0SE == 0x00){
            if(LetzteFlanke == 0){
                qDebug() << "Tmr0 Counter letzte Flanke =0";
            if(AktuelleFlanke != LetzteFlanke){
                qDebug() << "Tmr0 Counter letzte Flanke =0 Aktuell != Alt";
                regModel->reg[0][TMR0] = regModel->reg[0][TMR0]++;
            }

            }
        }


    //fallende Flanke
   if(Tmr0SE == 0x10){

    if(LetzteFlanke == 1){
    qDebug() << "Tmr0 Counter letzte Flanke =1";
       if(AktuelleFlanke != LetzteFlanke){
           qDebug() << "Tmr0 Counter letzte Flanke =1 Aktuell != Alt";
           regModel->reg[0][TMR0] = regModel->reg[0][TMR0]++;
       }
    }

    /*LetzteFlanke = AktuelleFlanke;
    PIC::Tmr0overflow();*/

   }
   }

    //1:PreScalerWert
    if(PreScalerTmr0 == true){
        qDebug() << "Tmr0 Counter PreScaler";
    //steigende Flanke 0->1
        if(Tmr0SE == 0x00){
            if(LetzteFlanke == 0){
                qDebug() << "Tmr0 Counter PreScaler letzte Flanke =0";
                if(AktuelleFlanke != LetzteFlanke){
                    qDebug() << "Tmr0 Counter letzte Flanke =0 != Aktuell";
                    PIC::Tmr0Increment();
                }

            }
        }


    //fallende Flanke
   if(Tmr0SE == 0x10){

        if(LetzteFlanke == 1){
            //qDebug() << "Tmr0 Counter PreScaler letzte Flanke =1";
            if(AktuelleFlanke != LetzteFlanke){
                //qDebug() << "Tmr0 Counter PreScaler letzte Flanke =1 != Aktuell";
                PIC::Tmr0Increment();
            }
        }
   }


   }

    LetzteFlanke = AktuelleFlanke;
    //PIC::Tmr0overflow();
}

void PIC::Tmr0Increment(){
    if(PreScalerCounter == ((PreScalerWert*2)-1)){
        //qDebug() << PreScalerCounter << "PreScalerCounter";
        int tempadd = regModel->reg[0][TMR0];
        tempadd++;
        regModel->reg[0][TMR0] = tempadd;
        PreScalerCounter = 0x00;
        //qDebug() << PreScalerCounter << "PreScalerCounter 0x00";
    }
}

void PIC::Tmr0overflow(){
    if(regModel->reg[0][TMR0] >= 0x100){
        regModel->reg[0][TMR0] = regModel->reg[0][TMR0] - 256;
        regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] | 0x04;
        PIC::ZBit(true);

    }


}

void PIC:: SyncSpecialReg(){
    int ziel = 0x00;
    if(bank == 0x00){
        ziel = 0x01;
    }
    if(bank == 0x01){
        ziel = 0x00;
    }

    regModel->reg[ziel][PCL] = regModel->reg[bank][PCL];
    regModel->reg[ziel][STATUS] = regModel->reg[bank][STATUS];
    regModel->reg[ziel][FSR] = regModel->reg[bank][FSR];
    regModel->reg[ziel][PCLATH] = regModel->reg[bank][PCLATH];
    regModel->reg[ziel][INTCON] = regModel->reg[bank][INTCON];

}

void PIC::IncrementCycles(){
    cycles++;
    LaufzeitCounter++;
    qDebug() << cycles << "cycles" << LaufzeitCounter << "LaufzeitCounter";
}

void PIC::LaufZeit(){
    Laufzeit = LaufzeitCounter / QuarzFreqzenz;
    Laufzeit = Laufzeit * 4;
    qDebug() << Laufzeit << "Laufzeit in Sekunden";
}

void PIC::InterruptAnalyzer(){
    int intcon = regModel->reg[bank][INTCON];
    int GIE = intcon & 0x80;
    int T0IE = intcon & 0x20;
    int T0IF = intcon & 0x04;
    int INTE = intcon & 0x10;
    int INTF = intcon & 0x02;
    int RBIE = intcon & 0x08;
    int RBIF = intcon & 0x01;

    if(GIE == 0x80){
        qDebug() << "GIE gesetzt" << GIE;
        if(T0IE == 0x20){
            qDebug() << "T0IE scharf" << T0IE;
            if(T0IF == 0x04){
                qDebug() << "T0IF gesetzt" << T0IF;
                RunInterrupt();
            }

        }
        if(RBIE == 0x08){
            qDebug() << "RBIE scharf" << RBIE;
            if(RBIF == 0x01){
                qDebug() << "RBIF gesetzt" << RBIF;
                RunInterrupt();
            }
        }
        if(INTE == 0x10){
            qDebug() << "INTE extern scharf" << INTE;
            if(INTF == 0x02){
                qDebug() << "INTF Flag gesetzt" << INTF;
                RunInterrupt();
            }
        }

    }
}

void PIC::RunInterrupt(){
    //GIE claeren -> Verunden mit 0x80 Kehrwert
    int disableGIE = 0x80 ^ 0xFF;
    int GIE = regModel->reg[bank][INTCON];
    GIE = GIE & disableGIE;
    regModel->reg[bank][INTCON] = GIE;
    //PC = 0004h
    k = 0x04;
    PIC::CALL();
}

void PIC::RBPeakAnalyzer(){
    int peakMode = regModel->reg[1][OPTION];
    peakMode = peakMode & 0x40;
    int portB = regModel->reg[0][PORTB];

    //steigende Flanke
    if(peakMode == 0x40){
        for(int i=0; i<=7; i++){
            //Alter Wert = 0
            if(RBAlt[i] == 0){
                //Aktuelle Werte maskieren
                for(int i=0; i<=7; i++){
                    int ref = pow(2,i);
                    RBAktuell[i] = portB & ref;
                    qDebug() << RBAktuell[i] << "RBAKTUELL" << portB<<"PortB";
                }
                //Bits auf Änderung Prüfen
                qDebug() << RBAlt[0] << RBAktuell[0] << "RBALT, Aktuell steigend";
                if(RBAlt[0] != RBAktuell[0]){
                    SetINTFFlag();
                }
                for(i=1; i<=7; i++){
                    if(RBAlt[i] != RBAktuell[i]){
                    SetRBInterruptFlag();
                    }
                }
            }
        }
    }

    //fallende Flanke
    if(peakMode == 0x00){
        for(int i=0; i<=7; i++){
            //Alter Wert = 0
            if(RBAlt[i] == 1){
                //Aktuelle Werte maskieren
                for(int i=0; i<=7; i++){
                    int ref = pow(2,i);
                    ref = ref ^ 0xFF;
                    RBAktuell[i] = portB & ref;
                    qDebug() << RBAktuell[i] << "RBAKTUELL" << portB<<"PortB";
                }
                //Bits auf Änderung Prüfen
                qDebug() << RBAlt[0] << RBAktuell[0] << "RBALT, Aktuell fallend";
                if(RBAlt[0] != RBAktuell[0]){
                    SetINTFFlag();
                }
                for(i=1; i<=7; i++){
                    if(RBAlt[i] != RBAktuell[i]){
                    SetRBInterruptFlag();
                    }
                }
            }
        }
    }
    for(int i=0; i<=7; i++){
        RBAlt[i] = RBAktuell[i];
        qDebug() << RBAlt[i] << "RBAlt" << i;
        qDebug() << RBAlt[i] << "RBAlt" << i;

        }
}

void PIC::SetINTFFlag(){
    int intcon = regModel->reg[bank][INTCON];
    intcon = intcon | 0x02;
    regModel->reg[bank][INTCON] = intcon;
    qDebug() << "INTF scharf";
}

void PIC::SetRBInterruptFlag(){
    int intcon = regModel->reg[bank][INTCON];
    intcon = intcon | 0x01;
    regModel->reg[bank][INTCON] = intcon;
}

void PIC::ExtClock(){
    int helper = 0;
    int tacktFlankeAktuell = regModel->reg[tacktAdresseBank][tacktAdresseZelle];
    helper = pow(2,tacktAdresseBit);
    tacktFlankeAktuell = tacktFlankeAktuell & helper;
    if(PIC::tackt == true){
            if(tacktFlankeAktuell == helper){
            qDebug() << "war gesetzt";
            qDebug() << tacktFlankeAktuell << "tfa";
            int folgetackt = 0x00;
            regModel->reg[tacktAdresseBank][tacktAdresseZelle] = regModel->reg[tacktAdresseBank][tacktAdresseZelle] & folgetackt;
            qDebug() << folgetackt << "folgetackt";
            }
            else{
            regModel->reg[tacktAdresseBank][tacktAdresseZelle] = regModel->reg[tacktAdresseBank][tacktAdresseZelle] | helper;
            qDebug() << "war nicht gesetzt" << helper;
            qDebug() << regModel->reg[tacktAdresseBank][tacktAdresseZelle];
            }
        }

    }
