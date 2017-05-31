#ifndef PWAITCONDITION_H
#define PWAITCONDITION_H

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

#endif // PWAITCONDITION_H
