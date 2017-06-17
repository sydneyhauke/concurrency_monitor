#ifndef READERWRITEREQUALCOND_H
#define READERWRITEREQUALCOND_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "owaitcondition.h"
#include "omutex.h"

class readerwriterequalcond : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OSemaphore mutex;
    OWaitCondition accessor;
    int nbReaders;
    bool first;

public:

    readerwriterequalcond() :
        accessor(1),
        fifo(1),
        mutex(1),
        firstReader(true),
        nbReaders(0)
    {}

    virtual void lockReader() {
        fifo.acquire();
        mutex.acquire();
        nbReaders++;
        if (nbReaders == 1 || !firstReader) {
            accessor.wait(&mutex);
        }
        firstReader = false;
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.wakeOne();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        fifo.acquire();

        if (!firstWriter) {
            accessor.wait(&mutex);
        }

        firstWriter = false;
    }

    virtual void unlockWriter() {
        fifo.release();
        accessor.wakeOne();
    }

};

#endif // READERWRITEREQUALCOND_H
