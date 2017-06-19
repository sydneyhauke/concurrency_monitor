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
    bool first;
    int nbReaders;
    int nbWriters;

public:

    readerwriterequalcond() :
        fifo(1),
        mutex(1),
        first(true),
        nbReaders(0),
        nbWriters(0)
    {}

    virtual void lockReader() {
        mutex.acquire();
        nbReaders++;
        mutex.release();

        fifo.acquire();
        mutex.acquire();

        if (!first) {
            accessor.wait(&mutex);
        }

        first = false;
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;

        if (nbReaders > 0 || nbWriters > 0) {
            accessor.wakeOne();
        } else {
            first = true;
        }

        mutex.release();
    }

    virtual void lockWriter() {
        mutex.acquire();
        nbWriters++;
        mutex.release();

        fifo.acquire();

        if (!first) {
            accessor.wait(&mutex);
        }

        first = false;
    }

    virtual void unlockWriter() {
        mutex.acquire();
        nbWriters--;
        mutex.release();

        fifo.release();

        if (nbReaders > 0 || nbWriters > 0) {
           accessor.wakeOne();
        } else {
            first = true;
        }
    }

};

#endif // READERWRITEREQUALCOND_H
