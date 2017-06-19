#ifndef READERWRITERPRIOWRITERSEM_H
#define READERWRITERPRIOWRITERSEM_H

#include "osemaphore.h"
#include "ireaderwriter.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterpriowriterSem : public IReaderWriter
{
protected:
    OSemaphore mutexReaders;
    OSemaphore mutexWriters;
    OSemaphore writer;
    OSemaphore reader;
    OSemaphore mutex;
    int nbReaders, nbWriters;

    WaitingLogger *wlInstance;

public:
    readerwriterpriowriterSem() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        reader(1),
        mutex(1),
        nbReaders(0),
        nbWriters(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::objectName(), "mutexReaders");
        mutexReaders.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutexReaders");

        wlInstance->addWaiting(QThread::objectName(), "reader");
        reader.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "reader");

        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbReaders++;
        if (nbReaders == 1) {
            wlInstance->addWaiting(QThread::objectName(), "writer");
            writer.acquire();
            wlInstance->removeWaiting(QThread::objectName(), "writer");
        }
        mutex.release();
        reader.release();
        mutexReaders.release();
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
        wlInstance->addWaiting(QThread::objectName(), "mutexWriters");
        mutexWriters.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "mutexWriters");

        nbWriters++;
        if (nbWriters == 1) {
            wlInstance->addWaiting(QThread::objectName(), "reader");
            reader.acquire();
            wlInstance->removeWaiting(QThread::objectName(), "reader");
        }
        mutexWriters.release();

        wlInstance->addWaiting(QThread::objectName(), "writer");
        writer.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "writer");
    }

    virtual void unlockWriter() {
        writer.release();
        mutexWriters.acquire();
        nbWriters--;
        if (nbWriters == 0) {
            reader.release();
        }
        mutexWriters.release();
    }

};

#endif // READERWRITERPRIOWRITERSEM_H
