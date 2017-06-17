#include "synchrocontroller.h"
#include "waitinglogger.h"
#include <iostream>

SynchroController::SynchroController() {}

SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause()
{
    monitorIn();
    wait(waitCond);
    monitorOut();
}

void SynchroController::resume()
{
    monitorIn();
    signal(waitCond);
    monitorOut();
}
