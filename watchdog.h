#ifndef WATCHDOG_H
#define WATCHDOG_H


class WatchDog
{
public:
    WatchDog();
    ~WatchDog();

bool WDflag = false;
int WDStartTime;


void SetStartTime();
void ActivateWDTimer();
void DisableWDTimer();
void WDTimer();
void RstSysState();

};

#endif // WATCHDOG_H
