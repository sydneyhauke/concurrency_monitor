#ifndef READERWRITERPRIOWRITERSEM_H
#define READERWRITERPRIOWRITERSEM_H

#include "osemaphore.h"
#include "ireaderwriter.h"

class readerwriterpriowriterSem
{
protected:
    OSemaphore mutexReaders;
    OSemaphore mutexWriters;
    OSemaphore writer;
    OSemaphore reader;
    OSemaphore mutex;
    int nbReaders, nbWriters;

public:
    readerwriterpriorwriterSem();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOWRITERSEM_H
