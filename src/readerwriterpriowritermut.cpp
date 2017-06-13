#include "readerwriterpriowritermut.h"

readerwriterpriowritermut::readerwriterpriowritermut() :
    mutexReaders(),
    mutexWriters(),
    writer(),
    reader(),
    mutex(),
    nbReaders(0),
    nbWriters(0)
{}

void readerwriterpriowritermut::lockReader() {
    mutexReaders.lock();
    reader.lock();
    mutex.lock();
    nbReaders++;
    if(nbReaders == 1) {
        writer.lock();
    }
    mutex.unlock();
    reader.unlock();
    mutexReaders.unlock();
}

void readerwriterpriowritermut::unlockReader() {
    mutex.lock();
    nbReaders--;
    if (nbReaders == 0) {
        writer.unlock();
    }
    mutex.unlock();
}

void readerwriterpriowritermut::lockWriter() {
    mutexWriters.lock();
    nbWriters++;
    if(nbWriters == 1) {
        reader.lock();
    }
    mutexWriters.unlock();
    writer.lock();
}

void readerwriterpriowritermut::unlockWriter() {
    writer.unlock();
    mutexWriters.lock();
    nbWriters--;
    if(nbWriters == 0) {
        reader.unlock();
    }
    mutexWriters.unlock();
}
