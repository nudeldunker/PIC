#include "interrupts.h"
#include "timer0interrupt.h"
#include "pic.h"

Interrupts::Interrupts()
{

}

Interrupts::~Interrupts()
{

}


//globale Interrupts
Interrupts::ChkGIE(bool){
    if(GIEState == true){
        return true;
    }
}

Interrupts::ChkIF(){
    if(Timer0Interrupt::T0IF == true){
        Timer0Interrupt::T0Interrupt();
    }
    //Ext-I if(){}
    //PortB-I if(){}
    //Data-EEPROM-I if(){}
}

SetGIE(){
    //INTCON(7) = 1
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] | 0x80;
}

ClrGIE(){
    //INTCON(7) = 0
    int 1ercomp = ~0x80;
    regModel->reg[bank][INTCON] = regModel->reg[bank][INTCON] & 1ercomp;
}
