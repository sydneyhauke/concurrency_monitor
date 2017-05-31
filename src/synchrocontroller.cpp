#include "synchrocontroller.h"

SynchroController::SynchroController()
{

}


SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause()
{

}

void SynchroController::resume()
{

}
