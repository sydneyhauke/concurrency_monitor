#include "readerwriterprioreadermut.h"

readerwriterprioreadermut::readerwriterprioreadermut() :
    mutexReaders(),
    mutexWriters(),
    writer(),
    nbReaders(0)
{}

readerwriterprioreadermut::lockReader() {
    mutexReaders.lock();
    nbReaders++;
    if(nbReader == 1) {
        writer.lock();
    }
    mutexReaders.unlock();
}

readerwriterprioreadermut::unlockReader() {
    mutexReaders.lock();
    nbReaders--;
    if(nbReaders == 0) {
        writer.unlock();
    }
    mutexReaders.unlock();
}

readerwriterprioreadermut::lockWriter() {
    mutexWriters.lock();
    writer.lock();
}

readerwriterprioreadermut::unlockWriter() {
    writer.unlock();
    mutexWriters.unlock();
}
