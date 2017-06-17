#ifndef READERWRITEREQUALMUT_H
#define READERWRITEREQUALMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterequalmut : public IReaderWriter
{
protected:
    OMutex mutex;
    OMutex fifo;
    OMutex writer;
    int nbReaders;

public:

    readerwriterequalmut() :
        mutex(1),
        fifo(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        fifo.lock();
        mutex.lock();
        nbReaders++;
        if (nbReaders == 1) {
            writer.lock();
        }
        mutex.unlock();
        fifo.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        nbReaders--;
        if (nbReaders == 0) {
            writer.unlock();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        fifo.lock();
        writer.lock();
    }

    virtual void unlockWriter() {
        writer.unlock();
        fifo.unlock();
    }


};

#endif // READERWRITEREQUALMUT_H
