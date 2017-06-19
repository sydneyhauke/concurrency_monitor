/**
  \file readerwriterprioreadingcond.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité en lecture avec des moniteurs de Mesa
*/

#ifndef READERWRITERPRIOREADINGCOND_H
#define READERWRITERPRIOREADINGCOND_H

#include "ireaderwriter.h"
#include "owaitcondition.h"

class readerwriterprioreadingcond : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OMutex mutex;
    OWaitCondition accessor;
    int nbReaders;

public:
    readerwriterprioreadingcond() :
        mutex(),
        accessor(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;
        if (nbReaders == 1) {
            accessor.wait(&mutex);
        }
        mutex.unlock();
    }

    virtual void unlockReader() {
        mutex.lock();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.wakeOne();
        }
        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();
        accessor.wait(&mutex);
        mutex.unlock();
    }

    virtual void unlockWriter() {
        mutex.lock();
        accessor.wait(&mutex);
        mutex.unlock();
    }

};

#endif // READERWRITERPRIOREADINGCOND_H
