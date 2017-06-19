#ifndef READERWRITERPRIOREADERMUT_H
#define READERWRITERPRIOREADERMUT_H

#include <QThread>

#include "omutex.h"
#include "ireaderwriter.h"
#include "waitinglogger.h"

class readerwriterprioreadermut : public IReaderWriter
{
protected:
    OMutex mutexReaders;
    OMutex mutexWriters;
    OMutex writer;
    int nbReaders;

public:
    readerwriterprioreadermut() :
        mutexReaders(),
        mutexWriters(),
        writer(),
        nbReaders(0)
    {}

    virtual void lockReader() {
        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "mutexReaders");
        mutexReaders.lock();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "mutexReaders");

        nbReaders++;
        if (nbReaders == 1) {
            WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "writer");
            writer.lock();
            WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "writer");
        }
        mutexReaders.unlock();
    }

    virtual void unlockReader() {
        mutexReaders.lock();
        nbReaders--;
        if(nbReaders == 0) {
            writer.unlock();
        }
        mutexReaders.unlock();
    }

    virtual void lockWriter() {
        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "mutexWriters");
        mutexWriters.lock();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "mutexWriters");

        WaitingLogger::getInstance()->addWaiting(QThread::currentThread()->objectName(), "writer");
        writer.lock();
        WaitingLogger::getInstance()->removeWaiting(QThread::currentThread()->objectName(), "writer");
    }

    virtual void unlockWriter() {
        writer.unlock();
        mutexWriters.unlock();
    }
};

#endif // READERWRITERPRIOREADERMUT_H
