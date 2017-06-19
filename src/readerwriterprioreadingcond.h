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
    OSemaphore mutex;
    OWaitCondition accessor;
    int nbReaders;

public:
    readerwriterprioreadingcond() :
        mutex(1),
        accessor(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutex.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            accessor.wait(&mutex);
        }
        mutex.release();
    }

    virtual void unlockReader() {
        mutex.acquire();
        nbReaders--;
        if (nbReaders == 0) {
            accessor.wakeOne();
        }
        mutex.release();
    }

    virtual void lockWriter() {
        mutex.acquire();
        accessor.wait(&mutex);
        mutex.release();
    }

    virtual void unlockWriter() {
        mutex.acquire();
        accessor.wait(&mutex);
        mutex.release();
    }

};

#endif // READERWRITERPRIOREADINGCOND_H
