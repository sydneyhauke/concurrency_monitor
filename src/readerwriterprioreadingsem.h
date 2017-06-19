#ifndef READERWRITERPRIOREADINGSEM_H
#define READERWRITERPRIOREADINGSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"

class readerwriterprioreadingsem : public IReaderWriter
{
protected:
    OSemaphore mutex;
    OSemaphore fifo;
    OSemaphore accessor;
    int nbReaders;
    int nbWriters;
    bool freeAllReaders;

public:
    readerwriterprioreadingsem() :
        mutex(1),
        fifo(1),
        accessor(1),
        nbReaders(0),
        nbWriters(0),
        freeAllReaders(false)
    {}

    virtual void lockReader() {
        mutex.acquire();
        nbReaders++;
        mutex.release();
        fifo.acquire();
        accessor.acquire();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;

        if (nbReaders > 0 && !freeAllReaders) {

            freeAllReaders = true;

            for (int i = 0; i < nbReaders + nbWriters; ++i)
                fifo.release();

            for (int i = 0; i < nbReaders; ++i)
                accessor.release();

        } else if (nbReaders == 0) {
            freeAllReaders = false;
            accessor.release();
            fifo.release();
        }

        mutex.release();
    }

    virtual void lockWriter() {
        mutex.acquire();
        nbWriters++;
        mutex.release();
        fifo.acquire();

        if (freeAllReaders)
            fifo.acquire();

        accessor.acquire();
    }

    virtual void unlockWriter() {
        accessor.release();
        fifo.release();
    }};

#endif // READERWRITERPRIOREADINGSEM_H
