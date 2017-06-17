/**
  \file cppmain.cpp
  \author Yosra Harbaoui et Denise Gemesio
  \date 26.04.2017
  \brief Classe main configurant les données de base des locomotives
  et démarrant le programme
  *
  * Nous n'avons ici pas besoin d'utiliser un mutex vu que le monitorIn/Out se charge
  * de ce rôle.
  *
*/

#ifndef READERWRITEREQUALHOARE_H
#define READERWRITEREQUALHOARE_H

#include "ireaderwriter.h"
#include "osemaphore.h"
#include "ohoaremonitor.h"


class readerwriterequalhoare : public IReaderWriter
{
protected:
    OSemaphore fifo;
    OHoareMonitor writer;
    OHoareMonitor::Condition accessing;
    int nbReaders;

public:
    readerwriterequalhoare() :
        fifo(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        monitorIn();
        fifo.acquire();
        nbReaders++;
        if (nbReaders == 1) {
            wait(accessing);
        }
        fifo.release();
        monitorOut();
    }

    virtual void unlockReader() {
        monitorIn();
        nbReaders--;
        if (nbReaders == 0) {
            OHoareMonitor::signal(accessing);
        }
        fifo.release();
        monitorOut();
    }

    virtual void lockWriter() {
        monitorIn();
        fifo.acquire();
        wait(accessing);
        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();
        OHoareMonitor::signal(accessing);
        fifo.release();
        monitorOut();
    }
};

#endif // READERWRITEREQUALHOARE_H
