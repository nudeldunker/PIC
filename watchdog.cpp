#include "watchdog.h"
#include "pic.h"
#include "time.h"

WatchDog::WatchDog()
{

}

WatchDog::~WatchDog()
{

}


WatchDog::SetStartTime(){
    WDStartTime = time(NULL);
}

WatchDog::ActivateWDTimer(){
    WDflag = true;
}

WatchDog::DisableWDTimer(){
    WDflag = false;
}

WatchDog::WDTimer(){
  int countsec = 18/1000; //runterzuzählende Zeit
  //hier dann die Eingabe von countsec
  while(WDflag){
  if(time(NULL) - start >= countsec){
    WatchDog::RstSysState();

}}}

WatchDog::RstSysState(){
    regModel->reg[bank][PCL] = regModel->reg[bank][PCL] & 0x00;
    PIC::runCode();
}
