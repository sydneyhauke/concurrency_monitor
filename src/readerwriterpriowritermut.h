/**
  \file readerwriterpriowritermut.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité rédacteur avec des mutex
*/

#ifndef READERWRITERPRIOWRITERMUT_H
#define READERWRITERPRIOWRITERMUT_H

#include "omutex.h"
#include "ireaderwriter.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterpriowritermut : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    OMutex reader;
    bool writing;
    int nbReaders, nbWriters;

    WaitingLogger *wlInstance;
public:
    readerwriterpriowritermut() :
        mutexReaders(1),
        mutexWriters(1),
        writer(1),
        reader(1),
        writing(false),
        nbReaders(0),
        nbWriters(0)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutexReaders");
        mutexReaders.lock();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutexReaders");

        nbReaders++;
        while (writing && nbWriters > 0) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "reader");
            reader.lock();
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "reader");
        }
        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;
        if (nbWriters > 0) {
            writer.unlock();
        } else {
            reader.unlock();
        }
        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "mutexWriters");
        mutexWriters.lock();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "mutexWriters");

        nbWriters++;
        if (nbWriters == 1) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
            writer.lock();
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }
        writing = true;
        mutexWriters.unlock();
    }

    virtual void unlockWriter() {
        mutexWriters.lock();
        writing = false;
        nbWriters--;
        if (nbWriters > 0) {
            writer.unlock();
        } else if (nbReaders > 0) {
            reader.unlock();
        }
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOWRITERMUT_H
