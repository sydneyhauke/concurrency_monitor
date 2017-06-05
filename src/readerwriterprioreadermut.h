#ifndef READERWRITERPRIOREADERMUT_H
#define READERWRITERPRIOREADERMUT_H

#include "omutex.h"
#include "ireaderwriter.h"

class readerwriterprioreadermut
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    int nbReaders;

public:
    readerwriterprioreadermut();
    virtual void lockReader();
    virtual void unlockReader();
    virtual void lockWriter();
    virtual void unlockWriter();
};

#endif // READERWRITERPRIOREADERMUT_H
