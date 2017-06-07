#include "readerwriterequalmut.h"

readerwriterequalmut::readerwriterequalmut() :
    mutex(),
    fifo(),
    writer(),
    nbReaders(0)
{}

readerwriterequalmut::lockReader() {
    fifo.lock();
    mutex.lock();
    nbReaders++;
    if(nbReaders++) {
        writer.lock();
    }
    mutex.unlock();
    fifo.unlock();
}

readerwriterequalmut::unlockReader() {
    mutex.lock();
    nbReaders--;
    if(nbReaders == 0) {
        writer.unlock();
    }
    mutex.unlock();
}

readerwriterequalmut::lockWriter() {
    fifo.lock();
    writer.lock();
}

readerwriterequalmut::unlockWriter() {
    writer.unlock();
    fifo.unlock();
}
