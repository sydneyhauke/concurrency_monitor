#ifndef READERWRITERPRIOREADERHOARE_H
#define READERWRITERPRIOREADERHOARE_H

#include <QThread>

#include "ireaderwriter.h"
#include "ohoaremonitor.h"
#include "waitinglogger.h"

class readerwriterprioreaderhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition writer, reader;
    int nbReaders, nbWriters;
    bool busy;

public:
    readerwriterprioreaderhoare() :
        nbReaders(0),
        nbWriters(0),
        busy(false)
    {}

    virtual void lockReader() {
        monitorIn();
        nbReaders++;
        if (busy) {
            WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "reader");
            wait(reader);
            WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "reader");
        }
        monitorOut();
    }

    virtual void unlockReader() {
        monitorIn();
        nbReaders--;
        if(nbReaders == 0) {
            signal(writer);
        }
        monitorOut();
    }

    virtual void lockWriter() {
        monitorIn();

        if (busy || nbReaders > 0) {
            WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "writer");
            wait(writer);
            WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }

        busy = true;
        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();
        busy = false;

        if (nbReaders > 0) {
            signal(reader);
        } else {
            // On essaie quand même de réveiller un writer,
            // même s'il n'y en a pas.
            signal(writer);
        }

        monitorOut();
    }
};

#endif // READERWRITERPRIOREADERHOARE_H
