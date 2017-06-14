#include "readerwriterequalmut.h"

readerwriterequalmut::readerwriterequalmut() :
    mutex(),
    fifo(),
    writer(),
    nbReaders(0)
{}

void readerwriterequalmut::lockReader() {
    fifo.lock();
    mutex.lock();
    nbReaders++;
    if(nbReaders++) {
        writer.lock();
    }
    mutex.unlock();
    fifo.unlock();
}

void readerwriterequalmut::unlockReader() {
    mutex.lock();
    nbReaders--;
    if(nbReaders == 0) {
        writer.unlock();
    }
    mutex.unlock();
}

void readerwriterequalmut::lockWriter() {
    fifo.lock();
    writer.lock();
}

void readerwriterequalmut::unlockWriter() {
    writer.unlock();
    fifo.unlock();
}
