#ifndef READERWRITERPRIOWRITERMUT_H
#define READERWRITERPRIOWRITERMUT_H

#include "omutex.h"
#include "ireaderwriter.h"

class readerwriterpriowritermut : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    OMutex reader;
    OMutex mutex;
    int nbReaders, nbWriters;

public:
    readerwriterpriowritermut() :
        mutexReaders(),
        mutexWriters(),
        writer(),
        reader(),
        mutex(),
        nbReaders(0),
        nbWriters(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();
        reader.lock();
        mutex.lock();
        nbReaders++;
        if(nbReaders == 1) {
            writer.lock();
        }
        mutex.unlock();
        reader.unlock();
        mutexReaders.unlock();
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
        mutexWriters.lock();
        nbWriters++;
        if(nbWriters == 1) {
            reader.lock();
        }
        mutexWriters.unlock();
        writer.lock();
    }

    virtual void unlockWriter() {
        writer.unlock();
        mutexWriters.lock();
        nbWriters--;
        if(nbWriters == 0) {
            reader.unlock();
        }
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOWRITERMUT_H
