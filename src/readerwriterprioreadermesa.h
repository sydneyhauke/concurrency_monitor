#ifndef READERWRITERPRIOREADERMESA_H
#define READERWRITERPRIOREADERMESA_H

#include "omutex.h"
#include "ireaderwriter.h"
#include "owaitcondition.h"

class readerwriterprioreadermesa
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int writers, readers;
    bool writerAccessing;

public:
    readerwriterprioreadermesa();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOREADERMESA_H
