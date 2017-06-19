#ifndef READERWRITERPRIOREADINGMUT_H
#define READERWRITERPRIOREADINGMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterprioreadingmut : public IReaderWriter
{
protected:

public:
    readerwriterprioreadingmut() :
    {}

    virtual void lockReader() {
    }

    virtual void unlockReader() {
    }

    virtual void lockWriter() {
    }

    virtual void unlockWriter() {
    }
};

#endif // READERWRITERPRIOREADINGMUT_H
