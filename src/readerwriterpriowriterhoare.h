#ifndef READERWRITERPRIOWRITERHOARE_H
#define READERWRITERPRIOWRITERHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"

class readerwriterpriowriterhoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition writer;
    Condition reader;
    int nbReadersAccessing, nbWriters;
    bool reading, writing;
public:
    readerwriterpriowriterhoare() :
        nbReadersAccessing(0),
        nbWriters(0),
        reading(false),
        writing(false)
    {}

    virtual void lockReader() {
        monitorIn();
        if (nbWriters > 0 || writing) {
            wait(reader);
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
            wait(writer);
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
