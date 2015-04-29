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

        qDebug() << k_long << "klong";
        qDebug() << pc << "PC";

        k_long=m_CmdList[pc] & 0x7FF;
        qDebug() << "CMDLIST" <<m_CmdList[pc];
        k=m_CmdList[pc] & 0xFF;
        qDebug() << k << "k";
        f=m_CmdList[pc] & 0x7F;
        qDebug() << f <<"f";
        d=m_CmdList[pc] & 0x80;
        d=(d>>7); //Test
        l=d;
        b=m_CmdList[pc] & 0x380;
        qDebug() << b << "b";

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
//}

}



void PIC::ADDWF(){
    qDebug() << "ADDWF";

erg= W+regModel->reg[bank][f];

/*if(erg > 255)
{
    CBit(true);
    erg=0;
}
else CBit(false);*/
ChkCBit(erg);

ChkZBit(erg);

/*ZBit checken. Warum abprüfen von W und erg?
 * if(regModel->reg[bank][W]<16 && erg>16)
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
else DCBit(false);*/
ChkDCBit(erg);
if(d==0)
{
    W=erg;
}
else regModel->reg[bank][f]=erg;

PC();

}

void PIC::ANDWF(){
    qDebug() << "ANDWF";

    int erg= W&&regModel->reg[bank][f];
    /*if(erg > 255)
    {
        CBit(true);
        erg=0;
    }
    else CBit(false);*/

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
    regModel->reg[bank][f]=0;
    ZBit(true);
    PC();
    //f  = 0x0;
    }

void PIC::CLRW(){
    qDebug() << "CLRW";

    //W = 0x0;

    //W = 0x0;
    W=0;
    ZBit(true);
    PC();
}

void PIC::COMF(){
    qDebug() << "COMF";

    erg = regModel->reg[bank][f];
    ChkZBit(erg);
    qDebug() << erg;
    erg = ~erg;
    qDebug() << erg;

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


    if(d==1){
        regModel->reg[bank][f]=erg;
        if(regModel->reg[bank][f]==0){
            PC();}
        else{NOP();}
    }
    else if(d==0){
        W=erg;
        if(W==0){
            PC();}else{NOP();}
    }



}

void PIC::INCF(){
    qDebug() << "INCF";

    erg = regModel->reg[bank][f];
    qDebug() << erg;
    erg = erg+1;
    qDebug() << erg;

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


    if(d==1){
     regModel->reg[bank][f]=erg;
     if(regModel->reg[bank][f]==0){
     PC();}else{NOP();}
     }
     else if(d==0){
     W=erg;
     if(W==0){
     PC();}else{NOP();}
     }

}

void PIC::IORWF(){
    qDebug() << "IORWF";

    qDebug() << regModel->reg[bank][f];
    qDebug() << W;
    erg=regModel->reg[bank][f]||W;
    qDebug() << erg;

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

    qDebug() << regModel->reg[bank][f];

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
    qDebug() << "W="<<W;
    regModel->reg[bank][f] = W;
    //f = W;
    qDebug() << "f="<< regModel->reg[bank][f];
    //qDebug() << "f="<< f;
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

    int carryset = regModel->reg[bank][f] && 0x80;
    int carryget = regModel->reg[bank][STATUS]&0x1;

    regModel->reg[bank][f] = regModel->reg[bank][f]*2;
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

    int carryset = regModel->reg[bank][f] && 0x80;
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

    ChkCBit(erg);
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
    erg = ~erg;
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
        PC();
    }else{NOP();}


}

void PIC::BTFSS(){
    qDebug() << "BTFSS";
    if(b==1){
        PC();
    }else{NOP();}


}

void PIC::ADDLW(){
    qDebug() << "ADDLW";

    //int erg = regModel->reg[bank][W] + k;
    erg = W + k;
    regModel->reg[bank][W]=erg;
    ChkCBit(erg);
    ChkDCBit(erg);
    ChkZBit(erg);
    W=erg;
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

    qDebug() << "pcl" << regModel->reg[bank][PCL];
    qDebug() << "stack" << stack[stackpointer];
    stack[stackpointer] = regModel->reg[bank][PCL] + 1;
    qDebug() << "stack" << stack[stackpointer];
    qDebug() << "stackpointer" << stackpointer;
    stackpointer++;
    qDebug() << "stackptr" << stackpointer;
    regModel->reg[bank][PCL] = k;
    qDebug() << "pcl" << regModel->reg[bank][PCL];
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
    W=erg;
    ChkZBit(erg);
    PC();
    }

void PIC::SUBLW(){
    qDebug() << "SUBLW1";

    erg =k - W;
    W=erg;
    ChkZBit(erg);
    PC();
}

void PIC::SLEEP(){
    qDebug() << "SLEEP";

    system("pause");
    PC();
}

void PIC::RETURN(){
    qDebug() << "RETURN";
    qDebug() << "pcl" << regModel->reg[bank][PCL];
    regModel->reg[bank][PCL] = stack[stackpointer];
    qDebug() << "pcl" << regModel->reg[bank][PCL];

    stackpointer--;

}

void PIC::RETURNLW(){
    qDebug() << "RETURNLW";
    W = k;
    RETURN();


}

void PIC::RETURNFIE(){
    qDebug() << "RETURNFIE";

    PC();

}

void PIC::MOVLW(){
    qDebug() << "MOVLW";

    W=k;
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

void PIC::teststackptr(){
    if(stackpointer <= 0){
        stackpointer = 7;
    }else if(stackpointer >= 7){
        stackpointer = 0;
    }
}

int PIC::ChkCBit(int){

    if(erg > 255)
    {
        CBit(true);
        return erg=0;
    }
    else CBit(false);

}

int PIC::ChkDCBit(int){

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
