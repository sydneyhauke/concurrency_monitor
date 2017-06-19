#ifndef READERWRITERPRIOWRITERCOND_H
#define READERWRITERPRIOWRITERCOND_H

#include "ireaderwriter.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterpriowritercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int nbWriters, nbReaders;
    bool readerAccessing;

    WaitingLogger *wlInstance;

public:
    readerwriterpriowritercond() :
        mutex(1),
        nbWriters(0),
        nbReaders(0),
        reader(0),
        writer(0),
        writerAccessing(false)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutex");

        nbReaders++;
        while (nbWriters > 0 || writerAccessing) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "reader");
            reader.wait(&mutex);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "reader");
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
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutex");

        nbWriters++;
        if (writerAccessing) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
            writer.wait(&mutex);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
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
