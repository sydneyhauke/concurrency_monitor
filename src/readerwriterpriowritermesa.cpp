#include "readerwriterpriowritermesa.h"

readerwriterpriowritermesa::readerwriterpriowritermesa() :
    mutex(1),
    writers(0),
    readers(0),
    writerAccessing(false)
{}

virtual void readerwriterpriowritermesa::lockReader() {
    mutex.lock();
    readers++;
    while (writers > 0 || writerAccessing) {
        reader.wait(&mutex);
    }
    mutex.unlock();
}

virtual void readerwriterpriowritermesa::unlockReader() {
    mutex.lock();
    readers--;
    if (writers > 0) {
        writer.wakeOne();
    }

    if (writers == 0 && readers > 0) {
        reader.wakeAll();
    }
    mutex.unlock();
}

virtual void readerwriterpriowritermesa::lockWriter() {
    mutex.lock();
    writers++;
    if (writerAccessing) {
        writer.wait(&mutex);
    }
    writerAccessing = true;
    mutex.unlock();
}

virtual void readerwriterpriowritermesa::unlockWriter() {
    mutex.lock();
    writers--;
    writerAccessing = false;
    if (writers > 0) {
        writer.wakeOne();
    }
    if (writers == 0 && readers > 0) {
        reader.wakeAll();
    }
    mutex.unlock();
}
