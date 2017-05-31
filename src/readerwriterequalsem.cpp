#include "readerwriterequalsem.h"

readerwriterequalsem::readerwriterequalsem() :
    mutex(1),
    fifo(1),
    writer(1),
    nbReaders(0)
{}

readerwriterequalsem::lockReader() {
    fifo.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders ++) {
        writer.acquire();
    }
    mutex.release();
    fifo.release();
}

readerwriterequalsem::unlockReader() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutex.release();
}

readerwriterequalsem::lockWriter() {
    fifo.acquire();
    writer.acquire();
}

readerwriterequalsem::unlockWriter() {
    writer.release();
    fifo.release();
}
