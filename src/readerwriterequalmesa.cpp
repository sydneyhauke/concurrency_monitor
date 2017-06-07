#include "readerwriterequalmesa.h"

readerwriterequalmesa::readerwriterequalmesa() :
    fifo(1),
    mutex(1),
    readers(0)
{}

virtual void readerwriterequalmesa::lockReader() {
    fifo.acquire();
    mutex.acquire();
    readers++;
    if (readers == 1) {
        writer.wait(&mutex);
    }
    mutex.unlock();
    fifo.release();
}

virtual void readerwriterequalmesa::unlockReader() {
    mutex.lock();
    readers--;
    if (readers == 0) {
        writer.wakeOne();
    }
    mutex.unlock();
}

virtual void readerwriterequalmesa::lockWriter() {
    fifo.acquire();
    writer.wait();
}

virtual void readerwriterequalmesa::unlockWriter() {
    writer.wakeOne();
    fifo.release();
}
