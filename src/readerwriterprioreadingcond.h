#ifndef READERWRITERPRIOREADINGCOND_H
#define READERWRITERPRIOREADINGCOND_H

#include "ireaderwriter.h"
#include "owaitcondition.h"

class readerwriterprioreadingcond : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OWaitCondition writer;
    int nbReaders;

public:
    readerwriterprioreadingcond() :
        mutexReaders(),
        writer(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutexReaders.lock();
        nbReaders++;

        if(nbReaders == 1) {
            wait(writer);
        }

        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;

        if(nbReaders == 0) {
            signal(writer);
        }

        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        wait(writer);
    }

    virtual void unlockWriter() {
        signal(writer);
    }

};

#endif // READERWRITERPRIOREADINGCOND_H
