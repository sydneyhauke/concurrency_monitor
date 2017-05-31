#include "readerwriterprioreadersem.h"

readerwriterprioreaderSem::readerwriterprioreaderSem() :

    mutexReaders(1),
    mutexWriters(1),
    writer(1),
    nbReaders(0)
{}

readerwriterprioreaderSem::lockReader() {
    mutexReaders.acquire();
    nbReaders++;
    if (nbReader == 1) {
        writer.acquire();
    }
    mutexReaders.release();
}

readerwriterprioreaderSem::unlockReader() {

}

readerwriterprioreaderSem::lockWriter() {

}

readerwriterprioreaderSem::unlockWriter() {

}

