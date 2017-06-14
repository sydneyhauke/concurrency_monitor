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
    OMutex mutex;
    OWaitCondition writer;
    int readers;

public:

    readerwriterequalcond() :
        fifo(1),
        mutex(1),
        readers(0)
    {}

    virtual void lockReader() {
        fifo.acquire();
        mutex.acquire();
        readers++;
        if (readers == 1) {
            writer.wait(&mutex);
        }
        mutex.unlock();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.lock();
        readers--;
        if (readers == 0) {
            writer.wakeOne();
        }
        mutex.unlock();
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
