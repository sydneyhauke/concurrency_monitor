/**
  \file readerwriterequalcond.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité égale avec des moniteurs de Mesa
*/

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
    bool first;
    int nbReaders;
    int nbWriters;

    WaitingLogger *wlInstance;

public:

    readerwriterequalcond() :
        fifo(1),
        mutex(1),
        first(true),
        nbReaders(0),
        nbWriters(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader()  {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutex");
        mutex.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutex");

        nbReaders++;
        mutex.release();

        // Ajout à la file d'attente
        wlInstance->addWaiting(QThread::objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::objectName(), "fifo");

        mutex.acquire();

        if (!first) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
            accessor.wait(&mutex);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "accessor");
        }

        first = false;
        mutex.release();
        fifo.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;

        if (nbReaders > 0 || nbWriters > 0) {
            accessor.wakeOne();
        } else {
            first = true;
        }

        mutex.release();
    }

    virtual void lockWriter() {
        mutex.acquire();
        nbWriters++;
        mutex.release();

        wlInstance->addWaiting(QThread::currentThread()->objectName(), "fifo");
        fifo.acquire();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "fifo");

        if (!first) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "accessor");
            accessor.wait(&mutex);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "accessor");
        }

        first = false;
    }

    virtual void unlockWriter() {
        mutex.acquire();
        nbWriters--;
        mutex.release();

        fifo.release();

        if (nbReaders > 0 || nbWriters > 0) {
           accessor.wakeOne();
        } else {
            first = true;
        }
    }

};

#endif // READERWRITEREQUALCOND_H
