#ifndef IREADERWRITER_H
#define IREADERWRITER_H

#include "synchrocontroller.h"

class IReaderWriter {
public:
    virtual void lockReader() = 0;
    virtual void unlockReader() = 0;
    virtual void lockWriter() = 0;
    virtual void unlockWriter() = 0;
};

#endif // IREADERWRITER_H
