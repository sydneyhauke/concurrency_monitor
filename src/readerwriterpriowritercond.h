#ifndef READERWRITERPRIOWRITERCOND_H
#define READERWRITERPRIOWRITERCOND_H

#include "ireaderwriter.h"

class readerwriterpriowritercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int writers, readers;
    bool readerAccessing;

public:
    readerwriterpriowritercond() :
        mutex(1),
        writers(0),
        readers(0),
        writerAccessing(false)
    {}

    virtual void lockReader() {
        mutex.lock();
        readers++;
        while (writers > 0 || writerAccessing) {
            reader.wait(&mutex);
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        readers--;
        if (writers > 0) {
            writer.wakeOne();
        }

        if (writers == 0 && readers > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();
        writers++;
        if (writerAccessing) {
            writer.wait(&mutex);
        }
        writerAccessing = true;
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        writers--;
        writerAccessing = false;
        if (writers > 0) {
            writer.wakeOne();
        }
        if (writers == 0 && readers > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOWRITERCOND_H
