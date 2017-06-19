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
