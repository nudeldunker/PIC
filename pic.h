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

    RegModel *regmodel;
    QList<int> m_CmdList;




    void ADDWF();
    void ANDWF();
    void CLRF();
    void CLRW();
    void COMF(f,d);
    void DECF(f,d);
    void DECFSZ(f,d);
    void INCF(f,d);
    void INCFSZ(f,d);
    void IORWF(f,d);
    void MOVF(f,d);
    void MOVWF(f);
    void NOP();
    void RLF(f,d);
    void RRF(f,d);
    void SUBWF(f,d);
    void SWAPF(f,d);
    void XORWF(f,d);
    void BCF(f,b);
    void BSF(f,b);
    void BTFSC(f,b);
    void BTFSS(f,b);
    void ADDLW(k);
    void ANDLW(k);
    void CALL(k_long);
    void CLRWDT();
    void GOTO(k_long);
    void XORLW();
    void SUBLW();
    void SUBLW();
    void SLEEP();
    void RETURN();
    void RETURNLW();
    void RETURNFIE();
    void MOVLW();
    void MOVLW();
    void MOVLW();
    void MOVLW();
    void IORLW();
signals:

public slots:
};

#endif // PIC_H
