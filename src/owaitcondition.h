/**
  \file owaitcondition.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Decorator declaration of a WaitCondition.
*/

#ifndef OWAITCONDITION_H
#define OWAITCONDITION_H

#include <QWaitCondition>
#include "omutex.h"

class OWaitCondition
{
private:
    QWaitCondition cond;

public:
    OWaitCondition();

    bool wait(OMutex *lockedMutex);

    void wakeAll();

    void wakeOne();

};

#endif // OWAITCONDITION_H
