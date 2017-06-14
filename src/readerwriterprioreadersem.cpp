#include "readerwriterprioreadersem.h"

readerwriterprioreaderSem::readerwriterprioreaderSem() :

    mutexReaders(1),
    mutexWriters(1),
    writer(1),
    nbReaders(0)
{}

void readerwriterprioreaderSem::lockReader() {
    mutexReaders.acquire();
    nbReaders++;
    if (nbReaders == 1) {
        writer.acquire();
    }
    mutexReaders.release();
}

void readerwriterprioreaderSem::unlockReader() {
    mutexReaders.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutexReaders.release();
}

void readerwriterprioreaderSem::lockWriter() {
    mutexWriters.acquire();
    writer.acquire();
}

void readerwriterprioreaderSem::unlockWriter() {
    writer.release();
    mutexWriters.release();
}

