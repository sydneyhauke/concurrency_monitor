#ifndef READERWRITERPRIOREADINGMUT_H
#define READERWRITERPRIOREADINGMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterprioreadingmut : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OMutex writer;
    int nbReaders;

public:
    readerwriterprioreadingmut() :
        mutexReaders(),
        writer(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();
        nbReaders++;

        if(nbReaders == 1) {
            writer.lock();
        }

        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;

        if(nbReaders == 0) {
            writer.lock();
        }

        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        writer.lock();
    }

    virtual void unlockWriter() {
        writer.unlock();
    }
};

#endif // READERWRITERPRIOREADINGMUT_H
