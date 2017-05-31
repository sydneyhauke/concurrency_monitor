#ifndef PWAITCONDITION_H
#define PWAITCONDITION_H

#include "omutex.h"

class OWaitCondition
{
public:
    OWaitCondition();

    bool wait(OMutex *lockedMutex);

    void wakeAll();

    void wakeOne();

};

#endif // PWAITCONDITION_H
