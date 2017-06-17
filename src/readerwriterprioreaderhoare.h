#ifndef READERWRITERPRIOREADERHOARE_H
#define READERWRITERPRIOREADERHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"

class readerwriterprioreaderhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition writer;
    int nbReaders;

public:
    readerwriterprioreaderhoare() :
        nbReaders(0)
    {}

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
        wait(writer);
        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();
        signal(writer);
        monitorOut();
    }
};

#endif // READERWRITERPRIOREADERHOARE_H
