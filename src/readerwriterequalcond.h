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
    OWaitCondition writer;
    int nbReaders;

public:

    readerwriterequalcond() :
        writer(1),
        fifo(1),
        mutex(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        fifo.acquire();
        mutex.acquire();
        nbReaders++;
        if (readers == 1) {
            writer.wait(&mutex);
        }
        mutex.release();
        fifo.release();
        SynchroController::getInstance()->pause();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            writer.wakeOne();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        fifo.acquire();
        writer.wait(&mutex);
    }

    virtual void unlockWriter() {
        writer.wakeOne();
        fifo.release();
    }

};

#endif // READERWRITEREQUALCOND_H
