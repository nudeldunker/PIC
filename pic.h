#ifndef PIC_H
#define PIC_H

#include <QObject>
#include "regmodel.h"
#include "regmodeldlgt.h"

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

    //für CALL
    int stackpointer = 0;
    int stack[7];

 void ChkZBit(int);
 void ChkDCBit(int);
 int ChkCBit(int);
    bool stop=true;
    bool singleStep=false;

 void CheckIndirect();


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

 void teststackptr();
 void pushstack();
 void popstack();

 void SetPSA();
 void ClearPSA();
 void SetPS000();
 void SetPS001();
 void SetPS010();
 void SetPS011();
 void SetPS100();
 void SetPS101();
 void SetPS110();
 void SetPS111();



 int getPC();
signals:
 void pointer();
 void finished();
 void breakPoint();

public slots:
 void init();
 void runCode();
 void updateReg();
 void stopExec(bool stop);
 void decodeCmd(int pc);


 void finish();
};

#endif // PIC_H
