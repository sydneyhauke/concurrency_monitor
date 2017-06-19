#ifndef READERWRITERPRIOREADINGCOND_H
#define READERWRITERPRIOREADINGCOND_H

#include "ireaderwriter.h"
#include "owaitcondition.h"

class readerwriterprioreadingcond : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OSemaphore mutex;
    OWaitCondition accessor;
    int nbReaders;

public:
    readerwriterprioreadingcond() :
        fifo(1),
        mutex(1),
        writer(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutex.lock();

        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();

        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();

        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();

        mutex.unlock();
    }

};

#endif // READERWRITERPRIOREADINGCOND_H
