#ifndef READERWRITEREQUALMUT_H
#define READERWRITEREQUALMUT_H

#include "ireaderwriter.h"
#include "omutex.h"

class readerwriterequalmut : public IReaderWriter
{
protected:
    OMutex mutex;
    OMutex fifo;
    OMutex writer;
    int nbReaders;

public:
    readerwriterequalmut();
    void lockReader();
    void unlockReader();
    void lockWriter();
    void unlockWriter();
};

#endif // READERWRITEREQUALMUT_H
