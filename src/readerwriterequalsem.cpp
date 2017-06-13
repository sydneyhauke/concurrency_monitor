#include "readerwriterequalsem.h"

readerwriterequalsem::readerwriterequalsem() :
    mutex(1),
    fifo(1),
    writer(1),
    nbReaders(0)
{}

void readerwriterequalsem::lockReader() {
    fifo.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders == 1) {
        writer.acquire();
    }
    mutex.release();
    fifo.release();
}

void readerwriterequalsem::unlockReader() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutex.release();
}

void readerwriterequalsem::lockWriter() {
    fifo.acquire();
    writer.acquire();
}

void readerwriterequalsem::unlockWriter() {
    writer.release();
    fifo.release();
}
