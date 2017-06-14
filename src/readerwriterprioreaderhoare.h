#ifndef READERWRITERPRIOREADERHOARE_H
#define READERWRITERPRIOREADERHOARE_H

#include "ireaderwriter.h"


class readerwriterprioreaderhoare : public IReaderWriter
{
public:
    readerwriterprioreaderhoare();

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

#endif // READERWRITERPRIOREADERHOARE_H
