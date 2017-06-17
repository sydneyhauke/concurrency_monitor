#ifndef READERWRITERPRIOREADERSEM_H
#define READERWRITERPRIOREADERSEM_H

#include "osemaphore.h"
#include "ireaderwriter.h"

class readerwriterprioreaderSem : public IReaderWriter
{
protected:
    OSemaphore mutexReaders;
    OSemaphore mutexWriters;
    OSemaphore writer;
    int nbReaders;

public:
    readerwriterprioreaderSem() :

        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutexReaders.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            writer.acquire();
        }
        mutexReaders.release();
    }

    virtual void unlockReader() {
        mutexReaders.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            writer.release();
        }
        mutexReaders.release();
    }

    virtual void lockWriter() {
        mutexWriters.acquire();
        writer.acquire();
    }

    virtual void unlockWriter() {
        writer.release();
        mutexWriters.release();
    }

};

#endif // READERWRITERPRIOREADERSEM_H
