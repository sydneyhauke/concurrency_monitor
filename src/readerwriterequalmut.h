#ifndef READERWRITEREQUALMUT_H
#define READERWRITEREQUALMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterequalmut
{
protected:
    OMutex mutex;
    OMutex fifo;
    OMutex writer;
    int nbReaders;

public:
    readerwriterequalmut();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITEREQUALMUT_H
