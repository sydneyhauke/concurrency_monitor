#ifndef READERWRITERPRIOREADINGSEM_H
#define READERWRITERPRIOREADINGSEM_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterprioreadingsem : public IReaderWriter
{
protected:
    OSemaphore mutex;
    OSemaphore fifo;
    OSemaphore accessor;
    int nbReaders;
    int nbWriters;
    bool freeAllReaders;

    WaitingLogger *wlInstance;

public:
    readerwriterprioreadingsem() :
        mutex(1),
        fifo(1),
        accessor(1),
        nbReaders(0),
        nbWriters(0),
        freeAllReaders(false)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbReaders++;
        mutex.release();

        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        wlInstance->addWaiting(QThread::objectName(), "accessor");
        accessor.acquire();
        wlInstance->addWaiting(QThread::objectName(), "accessor");
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;

        if (nbReaders > 0 && !freeAllReaders) {

            freeAllReaders = true;

            for (int i = 0; i < nbReaders + nbWriters; ++i)
                fifo.release();

            for (int i = 0; i < nbReaders; ++i)
                accessor.release();

        } else if (nbReaders == 0) {
            freeAllReaders = false;
            accessor.release();
            fifo.release();
        }

        mutex.release();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbWriters++;
        mutex.release();

        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        if (freeAllReaders) {
            wlInstance->addWaiting(QThread::objectName(), "fifo");
            fifo.acquire();
            wlInstance->removeWaiting(QThread::objectName(), "fifo");
        }

        wlInstance->addWaiting(QThread::objectName(), "accessor");
        accessor.acquire();
        wlInstance->addWaiting(QThread::objectName(), "accessor");
    }

    virtual void unlockWriter() {
        accessor.release();
        fifo.release();
    }};

#endif // READERWRITERPRIOREADINGSEM_H
