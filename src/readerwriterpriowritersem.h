#ifndef READERWRITERPRIOWRITERSEM_H
#define READERWRITERPRIOWRITERSEM_H

#include "osemaphore.h"
#include "ireaderwriter.h"

class readerwriterpriowriterSem : public IReaderWriter
{
protected:
    OSemaphore mutexReaders;
    OSemaphore mutexWriters;
    OSemaphore writer;
    OSemaphore reader;
    OSemaphore mutex;
    int nbReaders, nbWriters;

public:
    readerwriterpriowriterSem() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        reader(1),
        mutex(1),
        nbReaders(0),
        nbWriters(0)
    {}

    virtual void lockReader() {
        mutexReaders.acquire();
        reader.acquire();
        mutex.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            SynchroController.getInstance()->pause();
            writer.acquire();
        }
        mutex.release();
        reader.release();
        mutexReaders.release();
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
        mutexWriters.acquire();
        nbWriters++;
        if (nbWriters == 1) {
            reader.acquire();
        }
        mutexWriters.release();
        writer.acquire();
    }

    virtual void unlockWriter() {
        writer.release();
        mutexWriters.acquire();
        nbWriters--;
        if (nbWriters == 0) {
            reader.release();
        }
        mutexWriters.release();
    }

};

#endif // READERWRITERPRIOWRITERSEM_H
