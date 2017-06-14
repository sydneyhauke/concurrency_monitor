#ifndef READERWRITERPRIOWRITERHOARE_H
#define READERWRITERPRIOWRITERHOARE_H

#include "ireaderwriter.h"

class readerwriterpriowriterhoare : public IReaderWriter
{
public:
    readerwriterpriowriterhoare();

    virtual void lockReader() {
        monitorIn();

        monitorOut();
    }

    virtual void unlockReader() {
        monitorIn();

        monitorOut();
    }

    virtual void lockWriter() {
        monitorIn();

        monitorOut();
    }

    virtual void unlockWriter() {
        monitorIn();

        monitorOut();
    }
};

#endif // READERWRITERPRIOWRITERHOARE_H
