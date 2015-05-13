#ifndef PIC_H
#define PIC_H

#include <QObject>
#include "regmodel.h"
#include "regmodeldlgt.h"
#include "strings.h"

typedef QVector<int> regs;


class PIC : public QObject
{
    Q_OBJECT
public:
    explicit PIC(QObject *parent = 0);
    ~PIC();
    int k_long=0;
    int k=0;
    int f=0;
    int d=0;
    int l=0;
    int b=0;



    int bank=0;

    int W=0;

    int erg = 0;

    //RegModel *regmodel;
    QList<int> m_CmdList;

    RegModel *regModel;
    RegModelDlgt *regModelDlgt;



    int cycles = 0; //hier werden die durclaufenen Programmzyklen gezählt

    //Prescaler
    int PreScalerWert = 0;
    int PreScalerCounter = 0;
    bool PreScalerTmr0 = false;

    //Tmr0
    int LetzteFlanke = 0;

    //CALL / RETURN
    int stackpointer = 0;
    int stack[7];

    //Laufzeit
    double QuarzFreqzenz = 1000000;
    double Laufzeit = 0;
    int LaufzeitCounter = 0;

    //externer Tackt
    bool tackt = true;
    int tacktAdresseBank = 0;
    int tacktAdresseZelle = 0x06;
    int tacktAdresseBit = 0;
    int tacktflankealt = 0;

    //RBBuffer
    int RBAlt[8];

    int RBAktuell[8];

    bool stop=true;
    bool singleStep=false;


 void ChkZBit(int);
 void ChkDCBit(int);
 int ChkCBit(int);

 void InterruptAnalyzer();
 void RunInterrupt();
 void RBPeakAnalyzer();
 void SetRBInterruptFlag();
 void SetINTFFlag();

 void ExtClock();

 void getPreScaler();

 void LaufZeit();
 void ResetLaufZeit();
 void IncrementCycles();

 void setTmr0();
 void Tmr0Timer();
 void Tmr0Counter();
 void Tmr0overflow();
 void Tmr0Increment();

 void ChkIndirect();
 void SetBank();
 void SetRegister();
 void SyncSpecialReg();

 void decodeCmd();
 void ADDWF();
 void ANDWF();
 void CLRF();
 void CLRW();
 void COMF();
 void DECF();
 void DECFSZ();
 void INCF();
 void INCFSZ();
 void IORWF();
 void MOVF();
 void MOVWF();
 void NOP();
 void RLF();
 void RRF();
 void SUBWF();
 void SWAPF();
 void XORWF();
 void BCF();
 void BSF();
 void BTFSC();
 void BTFSS();
 void ADDLW();
 void ANDLW();
 void CALL();
 void CLRWDT();
 void GOTO();
 void XORLW();
 void SUBLW();
 void SLEEP();
 void RETURN();
 void RETURNLW();
 void RETURNFIE();
 void MOVLW();
 void IORLW();
 void PC();
 void RP0Bit(bool set);
 void TOBit(bool set);
 void PDBit(bool set);
 void ZBit(bool set);
 void DCBit(bool set);
 void CBit(bool set);

 void pushstack();
 void popstack();

 void SetPSA();
 void ClearPSA();




 int getPC();
signals:
 void pointer();
 void finished();

public slots:
 void init();
 void runCode();
 void updateReg();
 void stopExec(bool stop);
 void decodeCmd(int pc);


 void finish();
};

#endif // PIC_H
