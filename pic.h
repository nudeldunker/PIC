#ifndef PIC_H
#define PIC_H

#include <QObject>
#include "regmodel.h"

class PIC : public QObject
{
    Q_OBJECT
public:
    explicit PIC(QObject *parent = 0);
    ~PIC();
    int k_0;
    int k=0;
    int f=0;
    int d=0;
    int l=0;
    int b=0;

    //RegModel *regmodel;
    QList<int> m_CmdList;



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
signals:

public slots:
};

#endif // PIC_H
