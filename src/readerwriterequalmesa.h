#ifndef READERWRITEREQUALMESA_H
#define READERWRITEREQUALMESA_H

#include "osemaphore.h"
#include "omutex.h"
#include "owaitcondition.h"
#include "ireaderwriter.h"


class readerwriterequalmesa
{
protected:
    OSemaphore fifo;
    OMutex mutex;
    OWaitCondition writer;
    int readers;

public:
    readerwriterequalmesa();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITEREQUALMESA_H
