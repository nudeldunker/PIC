#include "timer0interrupt.h"
#include "interrupts.h"
#include "pic.h"

Timer0Interrupt::Timer0Interrupt()
{

}

Timer0Interrupt::~Timer0Interrupt()
{

}

//Timer 0 Interrupt


Timer0Interrupt::T0Interrupt(){
Interrupts::ChkGIE(GIEGO);
if(GIEGO == true){
    if(T0IE == true){
            qDebug << "Timer0 Interrupt kann laufen";
            Interrupts::ClrGIE();
            Timer0Interrupt::ClrT0IF();
            PIC::teststackptr();
            PIC::stack[stackpointer]=regModel->reg[bank][PCL];
            PIC::stackpointer++;
            regModel->reg[bank][PCL] = 0x0004;
        } else PIC::NOP();
}

}



Timer0Interrupt::SetT0IE(){
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] | 0x20;
    T0IE = true;
}

Timer0Interrupt::ClrT0IE(){
    int 1ercomp = ~0x20;
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] & 1ercomp;
    T0IE = false;
}

Timer0Interrupt::SetT0IF(){
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] | 0x04;
    T0IF = true;
}

Timer0Interrupt::ClrT0IF(){
    int 1ercomp = ~0x04;
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] & 1ercomp;
    T0IF = false;
}

Timer0Interrupt::Tmr0Overflow(){
    if(regModel->reg[bank][TMR0] == 0xFF){
        regModel->reg[bank][INTCON] = 0x00;
        Tmr0OfI();
    }
    if(regModel->reg[bank][TMR0] == 0x00){
        regModel->reg[bank][INTCON] = 0xFF;
        Tmr0OfI();
    }
}

