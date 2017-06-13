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
        mutexReaders(),
        mutexWriters(),
        writer(),
        nbReaders(0)
    {}

    void lockReader() {
        mutexReaders.lock();
        nbReaders++;
        if(nbReaders == 1) {
            writer.lock();
        }
        mutexReaders.unlock();
    }

    void unlockReader() {
        mutexReaders.lock();
        nbReaders--;
        if(nbReaders == 0) {
            writer.unlock();
        }
        mutexReaders.unlock();
    }

    void lockWriter() {
        mutexWriters.lock();
        writer.lock();
    }

    void unlockWriter() {
        writer.unlock();
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOREADERMUT_H
