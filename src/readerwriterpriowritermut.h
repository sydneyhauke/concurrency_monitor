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
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        reader(1),
        mutex(1),
        nbReaders(0),
        nbWriters(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();

        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();

        mutexReaders.unlock();
    }

    virtual void lockWriter() {

    }

    virtual void unlockWriter() {

    }
};

#endif // READERWRITERPRIOWRITERMUT_H
