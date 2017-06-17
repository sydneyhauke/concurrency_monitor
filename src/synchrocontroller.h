#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H

#include <ohoaremonitor.h>

class SynchroController : public OHoareMonitor
{
private:
    OHoareMonitor::Condition waitCond;

    SynchroController();
public:
    static SynchroController *getInstance();

    void pause();

    void resume();
};

#endif // SYNCHROCONTROLLER_H
