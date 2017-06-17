#ifndef READERWRITERPRIOWRITERHOARE_H
#define READERWRITERPRIOWRITERHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"

class readerwriterpriowriterhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition writer;
    Condition reader;
    int nbReaders, nbWriters;
public:
    readerwriterpriowriterhoare();

    virtual void lockReader() {
        monitorIn();
        nbReaders++;
        if(nbReaders == 1) {
            wait(writer);
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
        nbWriters++;
        if(nbWriters == 1) {
            wait(reader);
        }
        monitorOut();
        wait(writer);
    }

    virtual void unlockWriter() {
        signal(writer);
        monitorIn();
        nbWriters--;
        if(nbWriters == 0) {
            signal(reader);
        }
        monitorOut();
    }
};

#endif // READERWRITERPRIOWRITERHOARE_H
