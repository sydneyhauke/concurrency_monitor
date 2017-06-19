#ifndef READERWRITERPRIOREADINGHOARE_H
#define READERWRITERPRIOREADINGHOARE_H

#include "ireaderwriter.h"
#include "ohoaremonitor.h"

class readerwriterprioreadinghoare : public IReaderWriter, public OHoareMonitor
{
protected:
    Condition fifo;
    Condition accessor;
    int nbReaders;
    int nbWriters;
    bool freeAllReaders;

public:
    readerwriterprioreadinghoare():
        fifo(1),
        accessor(1),
        nbReaders(0),
        nbWriters(0),
        freeAllReaders(false)
    {}

    virtual void lockReader() {
        monitorIn();
        nbReaders++;
        monitorOut();
        wait(accessor);
    }

    virtual void unlockReader() {
        monitorIn();
        nbReaders--;
        if (nbReaders > 0 && !freeAllReaders) {
            freeAllReaders = true;
            for (int i = 0 ; i < nbReaders + nbWriters ; ++i){
                signal(fifo);
            }
            for(int i = 0 ; i < nbReaders ; ++i){
                signal(accessor);
            }
            monitorOut();

        } else if (nbReaders == 0 ) {
            freeAllReaders = false;
            monitorOut();
            signal(accessor);
            signal(fifo);
        }
    }

    virtual void lockWriter(){
        monitorIn();
        nbWriters++;
        wait(fifo);
        if (freeAllReaders) {
            wait(fifo);
        }
        wait(accessor);
        monitorOut();
    }

    virtual void unlockWriter(){
        monitorIn();
        nbWriters--;
        monitorOut();
        signal(accessor);
        signal(fifo);
    }
};

#endif // READERWRITERPRIOREADINGHOARE_H
