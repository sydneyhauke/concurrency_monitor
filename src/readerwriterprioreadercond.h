#ifndef READERWRITERPRIOREADERCOND_H
#define READERWRITERPRIOREADERCOND_H

#include "ireaderwriter.h"

class readerwriterprioreadercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int writers, readers;
    bool writerAccessing;

public:

    readerwriterprioreadercond() :
        mutex(1),
        writers(0),
        readers(0),
        writerAccessing(false)
    {}

    virtual void lockReader() {
        mutex.lock();
        readers++;
        if (writerAccessing) {
            reader.wait(&mutex);
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        readers--;
        if (readers == 0) {
            writer.wakeOne();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();
        writers++;
        while (readers > 0 || writerAccessing) {
            writer.wait(&mutex);
        }
        writerAccessing = true;
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        writers--;
        writerAccessing = false;
        if (readers > 0) {
            reader.wakeAll();
        }

        if (readers == 0 && writers > 0) {
            writer.wakeOne();
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADERCOND_H
