#ifndef READERWRITEREQUALSEM_H
#define READERWRITEREQUALSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"

class readerwriterequalsem
{
protected:
    OSemaphore mutex;
    OSemaphore fifo;
    OSemaphore writer;
    int nbReaders;

public:
    readerwriterequalsem();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITEREQUALSEM_H
