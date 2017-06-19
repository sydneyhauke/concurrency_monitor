#ifndef READERWRITEREQUALSEM_H
#define READERWRITEREQUALSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterequalsem : public IReaderWriter
{
protected:
    OSemaphore mutex;
    OSemaphore fifo;
    OSemaphore writer;
    int nbReaders;

    WaitingLogger *wlInstance;

public:
    readerwriterequalsem() :
        mutex(1),
        fifo(1),
        writer(1),
        nbReaders(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "fifo");

        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutex");

        nbReaders++;
        if (nbReaders == 1) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
            writer.acquire();
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            writer.release();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "fifo");

        wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
        writer.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
    }

    virtual void unlockWriter() {
        writer.release();
        fifo.release();
    }
};

#endif // READERWRITEREQUALSEM_H
