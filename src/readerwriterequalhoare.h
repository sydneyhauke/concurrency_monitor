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


class readerwriterequalhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition fifo;
    Condition accessing;
    int nbReaders;
    bool writing;
    bool firstWriter;
    bool firstReader;

public:
    readerwriterequalhoare() :
        fifo(1),
        nbReaders(0),
        accessing(1),
        writing(false),
        firstWriter(true)
    {}

    virtual void lockReader() {
        if (!firstReader || !firstWriter ) {
            wait(fifo);
        }
        firstReader = false;
        monitorIn();
        nbReaders++;
        while (nbReaders == 1 && writing) {
            wait(accessing);
        }
        monitorOut();
        signal(fifo);
    }

    virtual void unlockReader() {
        monitorIn();
        nbReaders--;
        if (nbReaders == 0) {
            signal(accessing);
        }
        monitorOut();
    }

    virtual void lockWriter() {
        if (!firstWriter || !firstReader) {
            wait(fifo);
        }
        firstWriter = false;
        monitorIn();
        if (nbReaders > 0) {
            wait(accessing);
        }
        writing = true;
        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();
        writing = false;
        signal(accessing);
        monitorOut();
        signal(fifo);
    }
};

#endif // READERWRITEREQUALHOARE_H
