#ifndef TIMER0INTERRUPT_H
#define TIMER0INTERRUPT_H


class Timer0Interrupt
{
public:
    Timer0Interrupt();
    ~Timer0Interrupt();

    //T0-Interrupt
    bool GIEGO = false;
    bool T0IE = false;
    bool T0IF = false;

    void T0Interrupt();
    void SetT0IE();
    void ClrT0IE();
    void SetT0IF();
    void ClrT0IF();

    void Tmr0Overflow();


};

#endif // TIMER0INTERRUPT_H
