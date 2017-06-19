/**
  \file readerwriterprioreadersem.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité lecteur avec des sémaphores
*/

#ifndef READERWRITERPRIOREADERSEM_H
#define READERWRITERPRIOREADERSEM_H

#include <QThread>

#include "osemaphore.h"
#include "ireaderwriter.h"
#include "waitinglogger.h"

class readerwriterprioreaderSem : public IReaderWriter
{
protected:
    OSemaphore mutexReaders;
    OSemaphore mutexWriters;
    OSemaphore writer;
    int nbReaders;

public:
    readerwriterprioreaderSem() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "mutexReaders");
        mutexReaders.acquire();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "mutexReaders");

        nbReaders++;
        if (nbReaders == 1) {
            WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "writer");
            writer.acquire();
            WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }
        mutexReaders.release();
    }

    virtual void unlockReader() {
        mutexReaders.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            writer.release();
        }
        mutexReaders.release();
    }

    virtual void lockWriter() {
        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "mutexWriters");
        mutexWriters.acquire();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "mutexWriters");

        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "writer");
        writer.acquire();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "writer");
    }

    virtual void unlockWriter() {
        writer.release();
        mutexWriters.release();
    }

};

#endif // READERWRITERPRIOREADERSEM_H
