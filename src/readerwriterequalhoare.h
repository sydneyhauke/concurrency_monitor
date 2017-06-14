#ifndef READERWRITEREQUALHOARE_H
#define READERWRITEREQUALHOARE_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "ohoaremonitor.h"

class readerwriterequalhoare : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OSemaphore mutex;
    OHoareMonitor writer;
    int readers;

public:
    readerwriterequalhoare() :
        fifo(1),
        mutex(1),
        readers(0)
    {}

    virtual void lockReader() {
        monitorIn();

        monitorOut();
    }

    virtual void unlockReader() {
        monitorIn();

        monitorOut();
    }

    virtual void lockWriter() {
        monitorIn();

        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();

        monitorOut();
    }
};

#endif // READERWRITEREQUALHOARE_H
