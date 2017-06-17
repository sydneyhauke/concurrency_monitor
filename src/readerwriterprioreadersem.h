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
    readerwriterprioreaderSem();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOREADERSEM_H
