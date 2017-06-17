#ifndef READERWRITERPRIOREADERHOARE_H
#define READERWRITERPRIOREADERHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"

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
            wait(reader);
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
            wait(writer);
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
