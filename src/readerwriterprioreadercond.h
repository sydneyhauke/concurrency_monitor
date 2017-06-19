/**
  \file readerwriterprioreadercond.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité lecteur avec des moniteurs de Mesa
*/

#ifndef READERWRITERPRIOREADERCOND_H
#define READERWRITERPRIOREADERCOND_H

#include "ireaderwriter.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterprioreadercond : public IReaderWriter
{
protected:
    OWaitCondition reader;
    OWaitCondition writer;
    OMutex mutex;
    int nbWriters, nbReaders;
    bool writerAccessing;

    WaitingLogger *wlInstance;

public:

    readerwriterprioreadercond() :
        mutex(1),
        nbWriters(0),
        nbReaders(0),
        writerAccessing(false)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;
        if (writerAccessing) {
            reader.wait(&mutex);
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        nbReaders--;
        if (nbReaders == 0) {
            writer.wakeOne();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();
        nbWriters++;
        while (nbReaders > 0 || writerAccessing) {
            writer.wait(&mutex);
        }
        writerAccessing = true;
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        nbWriters--;
        writerAccessing = false;
        if (nbReaders > 0) {
            reader.wakeAll();
        }

        if (nbReaders == 0 && nbWriters > 0) {
            writer.wakeOne();
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADERCOND_H
