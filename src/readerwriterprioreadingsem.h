#ifndef READERWRITERPRIOREADINGSEM_H
#define READERWRITERPRIOREADINGSEM_H

#include "ireaderwriter.h"


class readerwriterprioreadingsem : public IReaderWriter
{
protected:
    QSemaphore mutexReaders;
    QSemaphore writer;
    int nbReaders;

public:
    readerwriterprioreadingsem() :
        mutexReaders(1),
        writer(1),
        nbReaders(0)
    {}

    virtual void lockReader() {
        mutexReaders.acquire();
        nbReaders++;

        if (nbReaders == 1) {
            writer.acquire();
        }

        mutexReaders.release();
    }

    virtual void unlockReader() {
        mutexReaders.acquire();
        nbReaders -= 1;

        if (nbReaders == 0) {
            writer.release();
        }

        mutexReaders.release();
    }

    virtual void lockWriter() {
        writer.acquire();
    }

    virtual void unlockWriter() {
        writer.release();
    }
};

#endif // READERWRITERPRIOREADINGSEM_H
