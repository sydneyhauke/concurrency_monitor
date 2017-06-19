/**
  \file synchrocontroller.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Single instance class that pauses a thread, typically when he enters a resource
*/

#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H

#include <ohoaremonitor.h>

class SynchroController : public OHoareMonitor
{
private:
    OHoareMonitor::Condition waitCond; // All threads calling pause() are waiting on this waitCondition

    SynchroController();
public:
    static SynchroController *getInstance();

    void pause();

    void resume();
};

#endif // SYNCHROCONTROLLER_H
