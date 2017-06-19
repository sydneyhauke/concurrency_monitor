/**
  \file readerwriterpriowritercond.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité rédacteur avec des moniteurs de Mesa
*/

#ifndef READERWRITERPRIOWRITERCOND_H
#define READERWRITERPRIOWRITERCOND_H

#include "ireaderwriter.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterpriowritercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int nbWriters, nbReaders;
    bool readerAccessing;

    WaitingLogger *wlInstance;

public:
    readerwriterpriowritercond() :
        mutex(1),
        nbWriters(0),
        nbReaders(0),
        reader(0),
        writer(0),
        writerAccessing(false)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbReaders++;
        while (nbWriters > 0 || writerAccessing) {
            wlInstance->addWaiting(QThread::objectName(), "reader");
            reader.wait(&mutex);
            wlInstance->removeWaiting(QThread::objectName(), "reader");
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        nbReaders--;
        if (nbWriters > 0) {
            writer.wakeOne();
        }

        if (nbWriters == 0 && nbReaders > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbWriters++;
        if (writerAccessing) {
            wlInstance->addWaiting(QThread::objectName(), "writer");
            writer.wait(&mutex);
            wlInstance->removeWaiting(QThread::objectName(), "writer");
        }
        writerAccessing = true;
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        nbWriters--;
        writerAccessing = false;

        if (nbWriters > 0) {
            writer.wakeOne();
        }

        if (nbWriters == 0 && nbReaders > 0) {
            reader.wakeAll();
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOWRITERCOND_H
