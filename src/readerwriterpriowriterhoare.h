/**
  \file readerwriterpriowriterhoare.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité rédacteur avec des moniteurs de Hoare
*/

#ifndef READERWRITERPRIOWRITERHOARE_H
#define READERWRITERPRIOWRITERHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterpriowriterhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition writer;
    Condition reader;
    int nbReadersAccessing, nbWriters;
    bool reading, writing;

    WaitingLogger *wlInstance;

public:
    readerwriterpriowriterhoare() :
        nbReadersAccessing(0),
        nbWriters(0),
        reading(false),
        writing(false)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        monitorIn();
        if (nbWriters > 0 || writing) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "reader");
            wait(reader);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "reader");
        }

        nbReadersAccessing++;
        reading = true;
        monitorOut();
    }

    virtual void unlockReader() {
        monitorIn();
        nbReadersAccessing--;

        if (nbWriters > 0 && nbReadersAccessing == 0) {
            reading = false;
            signal(writer);
        } else if (nbWriters == 0) {
            signal(reader);
        }
        monitorOut();
    }

    virtual void lockWriter() {
        monitorIn();
        nbWriters++;
        while (reading || writing) {
            wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
            wait(writer);
            wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }
        writing = true;
        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();
        writing = false;
        nbWriters--;
        if (nbWriters > 0) {
            signal(writer);
        } else {
            signal(reader);
        }
        monitorOut();
    }
};

#endif // READERWRITERPRIOWRITERHOARE_H
