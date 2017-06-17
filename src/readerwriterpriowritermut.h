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
    bool writing;
    int nbReaders, nbWriters;

public:
    readerwriterpriowritermut() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        reader(1),
        writing(false),
        nbReaders(0),
        nbWriters(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();
        nbReaders++;
        while (writing && nbWriters > 0) {
            reader.lock();
        }
        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;
        if (nbWriters > 0) {
            writer.unlock();
        } else {
            reader.unlock();
        }
        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        mutexWriters.lock();
        nbWriters++;
        if (nbWriters == 1) {
            writer.lock();
        }
        writing = true;
        mutexWriters.unlock();
    }

    virtual void unlockWriter() {
        mutexWriters.lock();
        writing = false;
        nbWriters--;
        if (nbWriters > 0) {
            writer.unlock();
        } else if (nbReaders > 0) {
            reader.unlock();
        }
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOWRITERMUT_H
