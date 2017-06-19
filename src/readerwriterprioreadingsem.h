#ifndef READERWRITERPRIOREADINGSEM_H
#define READERWRITERPRIOREADINGSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"

class readerwriterprioreadingsem : public IReaderWriter
{
protected:
    OSemaphore mutex;
    OSemaphore accessor;
    int nbReaders;
    int nbWriters;
    bool freeAllReaders;

public:
    readerwriterprioreadingsem() :
        mutex(1),
        accessor(1),
        nbReaders(0),
        nbWriters(0),
        freeAllReaders(false)
    {}

    virtual void lockReader() {
        mutex.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            accessor.acquire();
        }
        mutex.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.release();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        accessor.acquire();
    }

    virtual void unlockWriter() {
        accessor.release();
    }};

#endif // READERWRITERPRIOREADINGSEM_H
