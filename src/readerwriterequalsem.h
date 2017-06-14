#ifndef READERWRITEREQUALSEM_H
#define READERWRITEREQUALSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"

class readerwriterequalsem : public IReaderWriter
{
protected:
    OSemaphore mutex;
    OSemaphore fifo;
    OSemaphore writer;
    int nbReaders;

public:
    readerwriterequalsem() :
        mutex(1),
        fifo(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        fifo.acquire();
        mutex.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            writer.acquire();
        }
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            writer.release();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        fifo.acquire();
        writer.acquire();
    }

    virtual void unlockWriter() {
        writer.release();
        fifo.release();
    }
};

#endif // READERWRITEREQUALSEM_H
