#include "readerwriterequalsem.h"

readerwriterequalsem::readerwriterequalsem() :
    mutex(1),
    fifo(1),
    writer(1),
    nbReaders(0)
{}

virtual void readerwriterequalsem::lockReader() {
    fifo.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders == 1) {
        writer.acquire();
    }
    mutex.release();
    fifo.release();
}

virtual void readerwriterequalsem::unlockReader() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutex.release();
}

virtual void readerwriterequalsem::lockWriter() {
    fifo.acquire();
    writer.acquire();
}

virtual void readerwriterequalsem::unlockWriter() {
    writer.release();
    fifo.release();
}
