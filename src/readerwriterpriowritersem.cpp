#include "readerwriterpriowritersem.h"
#include "synchrocontroller.h"

readerwriterpriowritersem::readerwriterpriowritersem() :
    mutexReaders(1),
    mutexWriters(1),
    writer(1),
    reader(1),
    mutex(1),
    nbReaders(0),
    nbWriters(0)
{}

virtual void readerwriterpriowriterSem::lockReader() {
    mutexReaders.acquire();
    reader.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders == 1) {
        SynchroController.getInstance()->pause();
        writer.acquire();
    }
    mutex.release();
    reader.release();
    mutexReaders.release();
}

virtual void readerwriterpriowriterSem::unlockReader() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders == 0) {
        writer.release();
    }
    mutex.release();
}

virtual void readerwriterpriowriterSem::lockWriter() {
    mutexWriters.acquire();
    nbWriters++;
    if (nbWriters == 1) {
        reader.acquire();
    }
    mutexWriters.release();
    writer.acquire();
}

virtual void readerwriterpriowriterSem::unlockWriter() {
    writer.release();
    mutexWriters.acquire();
    nbWriters--;
    if (nbWriters == 0) {
        reader.release();
    }
    mutexWriters.release();
}
