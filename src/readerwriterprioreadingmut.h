#ifndef READERWRITERPRIOREADINGMUT_H
#define READERWRITERPRIOREADINGMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterprioreadingmut : public IReaderWriter
{
protected:
    OMutex mutex;
    OMutex accessor;
    int nbReaders;
    int nbWriters;
public:
    readerwriterprioreadingmut() :
    {}

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;
        mutex.unlock();
        accessor.lock();


    }

    virtual void unlockReader() {

    }

    virtual void lockWriter() {
        mutex.lock();
        nbWriters++;
        mutex.unlock();
    }

    virtual void unlockWriter() {

    }
};

#endif // READERWRITERPRIOREADINGMUT_H
