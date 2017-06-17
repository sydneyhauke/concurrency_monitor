#ifndef READERWRITERPRIOREADERMUT_H
#define READERWRITERPRIOREADERMUT_H

#include "omutex.h"
#include "ireaderwriter.h"

class readerwriterprioreadermut : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    int nbReaders;

public:
    readerwriterprioreadermut() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();
        nbReaders++;
        if (nbReaders == 1) {
            writer.lock();
        }
        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;
        if(nbReaders == 0) {
            writer.unlock();
        }
        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        mutexWriters.lock();
        writer.lock();
    }

    virtual void unlockWriter() {
        writer.unlock();
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOREADERMUT_H
