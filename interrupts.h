#ifndef INTERRUPTS_H
#define INTERRUPTS_H


class Interrupts
{
public:
    Interrupts();
    ~Interrupts();





    //globale Interrupts
    bool GIEState = false;

    bool ChkGIE(bool);
    void SetGIE();
    void ClrGIE();
    void ChkIF();

    };

#endif // INTERRUPTS_H
