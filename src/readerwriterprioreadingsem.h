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
    OSemaphore accessor;
    int nbReaders;
    int nbWriters;
    bool freeAllReaders;

    WaitingLogger *wlInstance;

public:
    readerwriterprioreadingsem() :
        mutex(1),
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
        if (nbReaders == 1) {
            wlInstance->addWaiting(QThread::objectName(), "accessor");
            accessor.acquire();
            wlInstance->addWaiting(QThread::objectName(), "accessor");
        }
        mutex.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.release();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "accessor");
        accessor.acquire();
        wlInstance->addWaiting(QThread::objectName(), "accessor");
    }

    virtual void unlockWriter() {
        accessor.release();
    }};

#endif // READERWRITERPRIOREADINGSEM_H
