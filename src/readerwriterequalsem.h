/**
  \file readerwriterequalsem.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité égale avec des sémaphores
*/

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
        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

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
        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        wlInstance->addWaiting(QThread::objectName(), "writer");
        writer.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "writer");
    }

    virtual void unlockWriter() {
        writer.release();
        fifo.release();
    }
};

#endif // READERWRITEREQUALSEM_H
