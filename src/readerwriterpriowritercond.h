#ifndef READERWRITERPRIOWRITERCOND_H
#define READERWRITERPRIOWRITERCOND_H

#include "ireaderwriter.h"

class readerwriterpriowritercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int nbWriters, nbReaders;
    bool readerAccessing;

public:
    readerwriterpriowritercond() :
        mutex(1),
        nbWriters(0),
        nbReaders(0),
        reader(0),
        writer(0),
        writerAccessing(false)
    {}

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;
        while (nbWriters > 0 || writerAccessing) {
            reader.wait(&mutex);
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        nbReaders--;
        if (nbWriters > 0) {
            writer.wakeOne();
        }

        if (nbWriters == 0 && nbReaders > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();
        nbWriters++;
        if (writerAccessing) {
            writer.wait(&mutex);
        }
        writerAccessing = true;
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        nbWriters--;
        writerAccessing = false;

        if (nbWriters > 0) {
            writer.wakeOne();
        }

        if (nbWriters == 0 && nbReaders > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOWRITERCOND_H
