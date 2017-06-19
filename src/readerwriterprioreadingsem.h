/**
  \file readerwriterprioreadingsem.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité en lecture avec des sémaphores
*/

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

    WaitingLogger *wlInstance;

public:
    readerwriterprioreadingsem() :
        mutex(1),
        accessor(1),
        nbReaders(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutex");

        nbReaders++;
        if (nbReaders == 1) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
            accessor.acquire();
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
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
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
        accessor.acquire();
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
    }

    virtual void unlockWriter() {
        accessor.release();
    }};

#endif // READERWRITERPRIOREADINGSEM_H
