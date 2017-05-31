#include "readerwriterprioreadersem.h"

readerwriterprioreaderSem::readerwriterprioreaderSem() :

    mutexReaders(1),
    mutexWriters(1),
    writer(1),
    nbReaders(0)
{}

virtual void readerwriterprioreaderSem::lockReader() {
    mutexReaders.acquire();
    nbReaders++;
    if (nbReader == 1) {
        writer.acquire();
    }
    mutexReaders.release();
}

virtual void readerwriterprioreaderSem::unlockReader() {
    mutexReaders.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutexReaders.release();
}

virtual void readerwriterprioreaderSem::lockWriter() {
    mutexWriters.acquire();
    writer.acquire();
}

virtual void readerwriterprioreaderSem::unlockWriter() {
    writer.release();
    mutexWriters.release();
}

