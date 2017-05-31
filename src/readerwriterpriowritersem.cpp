#include "readerwriterpriowritersem.h"

readerwriterpriowritersem::readerwriterpriowritersem() :
    mutexReaders(1),
    mutexWriters(1),
    writer(1),
    reader(1),
    mutex(1),
    nbReaders(0),
    nbWriters(0)
{}

readerwriterpriowriterSem::lockReader() {
    mutexReaders.acquire();
    reader.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders == 1) {
        writer.acquire();
    }
    mutex.release();
    reader.release();
    mutexReaders.release();
}

readerwriterpriowriterSem::unlockReader() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutex.release();
}

readerwriterpriowriterSem::lockWriter() {
    mutexWriters.acquire();
    nbWriters++;
    if (nbWriters == 1) {
        reader.acquire();
    }
    mutexWriters.release();
    writer.acquire();
}

readerwriterpriowriterSem::unlockWriter() {
    writer.release();
    mutexWriters.acquire();
    nbWriters--;
    if (nbWriters == 0) {
        reader.release();
    }
    mutexWriters.release();
}
