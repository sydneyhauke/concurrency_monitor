#ifndef READERWRITERPRIOWRITERMESA_H
#define READERWRITERPRIOWRITERMESA_H

#include "ireaderwriter.h"
#include "owaitcondition.h"

class readerwriterpriowritermesa
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int writers, readers;
    bool readerAccessing;

public:
    readerwriterpriowritermesa();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOWRITERMESA_H
