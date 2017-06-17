#include "synchrocontroller.h"
#include "waitinglogger.h"
#include <iostream>

SynchroController::SynchroController() : barrier(0)
{

}


SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause()
{
    barrier.acquire();
    std::cout << "Program paused" << std::endl;
    WaitingLogger::getInstance()->updateView();
}

void SynchroController::resume()
{
    barrier.release();
    std::cout << "Program resumed" << std::endl;
}
