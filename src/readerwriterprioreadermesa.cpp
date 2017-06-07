#include "readerwriterprioreadermesa.h"

readerwriterprioreadermesa::readerwriterprioreadermesa() :
    mutex(1),
    writers(0),
    readers(0),
    writerAccessing(false)
{}

virtual void readerwriterprioreadermesa::lockReader() {
    mutex.lock();
    readers++;
    if (writerAccessing) {
        reader.wait(&mutex);
    }
    mutex.unlock();
}

virtual void readerwriterprioreadermesa::unlockReader() {
    mutex.lock();
    readers--;
    if (readers == 0) {
        writer.wakeOne();
    }
    mutex.unlock();
}

virtual void readerwriterprioreadermesa::lockWriter() {
    mutex.lock();
    writers++;
    while (readers > 0 || writerAccessing) {
        writer.wait(&mutex);
    }
    writerAccessing = true;
    mutex.unlock();
}

virtual void readerwriterprioreadermesa::unlockWriter() {
    mutex.lock();
    writers--;
    writerAccessing = false;
    if (readers > 0) {
        reader.wakeAll();
    }

    if (readers == 0 && writers > 0) {
        writer.wakeOne();
    }
    mutex.unlock();
}
