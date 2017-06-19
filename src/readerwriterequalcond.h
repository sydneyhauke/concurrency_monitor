#ifndef READERWRITEREQUALCOND_H
#define READERWRITEREQUALCOND_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "owaitcondition.h"
#include "omutex.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterequalcond : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OSemaphore mutex;
    OWaitCondition accessor;
    int nbReaders;
    bool first;

    WaitingLogger *wlInstance;

public:

    readerwriterequalcond() :
        accessor(1),
        fifo(1),
        mutex(1),
        firstReader(true),
        nbReaders(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbReaders++;
        if (nbReaders == 1 || !firstReader) {
            wlInstance->addWaiting(QThread::objectName(), "accessor");
            accessor.wait(&mutex);
            wlInstance->removeWaiting(QThread::objectName(), "accessor");
        }
        firstReader = false;
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.wakeOne();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        if (!firstWriter) {
            wlInstance->addWaiting(QThread::objectName(), "accessor");
            accessor.wait(&mutex);
            wlInstance->removeWaiting(QThread::objectName(), "accessor");
        }

        firstWriter = false;
    }

    virtual void unlockWriter() {
        fifo.release();
        accessor.wakeOne();
    }

};

#endif // READERWRITEREQUALCOND_H
