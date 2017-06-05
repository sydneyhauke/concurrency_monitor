#ifndef READERWRITERPRIOWRITERMUT_H
#define READERWRITERPRIOWRITERMUT_H

#include "omutex.h"
#include "ireaderwriter.h"

class readerwriterpriowritermut
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    OMutex reader;
    OMutex mutex;
    int nbReaders, nbWriters;

public:
    readerwriterpriowritermut();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOWRITERMUT_H
