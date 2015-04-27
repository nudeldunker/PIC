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

    //RegModel *regmodel;
    QList<int> m_CmdList;

    RegModel *regModel;
    RegModelDlgt *regModelDlgt;

    int W;

    bool stop=true;
    bool singleStep=false;




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
 void SUBLW1();
 void SUBLW2();
 void SLEEP();
 void RETURN();
 void RETURNLW();
 void RETURNFIE();
 void MOVLW1();
 void MOVLW2();
 void MOVLW3();
 void MOVLW4();
 void IORLW();
 void PC();
 void RP0Bit(bool set);
 void TOBit(bool set);
 void PDBit(bool set);
 void ZBit(bool set);
 void DCBit(bool set);
 void CBit(bool set);

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
