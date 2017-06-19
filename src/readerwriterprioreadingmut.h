#ifndef READERWRITERPRIOREADINGMUT_H
#define READERWRITERPRIOREADINGMUT_H

#include "ireaderwriter.h"
#include "omutex.h"
#include "waitinglogger.h"

#include <QThread>

class readerwriterprioreadingmut : public IReaderWriter {
protected:
    OMutex mutex;
    OMutex accessor;
    int nbReaders;
    std::vector<int> waitingIds;
    int currentId;
    int toRealeaseId;
    bool readerAccessing;
    bool first;

    WaitingLogger *wlInstance;

public:

    readerwriterprioreadingmut() :
        mutex(1),
        accessor(1),
        nbReaders(0),
        currentId(0),
        toRealeaseId(-1),
        readerAccessing(false),
        first(true)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        nbReaders++;

        if (!first && !readerAccessing) {
            int id = currentId++;
            waitingIds.push_back(id);

            while (!first && !readerAccessing && (toRealeaseId!= id)) {
                wlInstance->addWaiting(QThread::objectName(), "accessor");
                accessor.lock();
                wlInstance->removeWaiting(QThread::objectName(), "accessor");
            }
        }

        // Si on est dans le cas où un reader est en train d'accéder
        if (readerAccessing) {
            waitingIds.erase(std::find(waitingIds.begin(), waitingIds.end(), id));
        }

        first = false;
        readerAccessing = true;

        mutex.unlock();
    }

    virtual void unlockReader() {

        mutex.lock();
        nbReaders--;

        if (waitingIds.size() > 0) {
            toRealeaseId = waitingIds[0];
            waitingIds.erase(0);
            accessor.unlock();
        } else if (nbReaders == 0) {
            readerAccessing = false;
        } else {
            first = true;
        }

        mutex.unlock();
    }

    virtual void lockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        if (!first) {
            int id = currentId++;
            waitingIds.push_back(id);

            while (toRealeaseId!= id) {
                wlInstance->addWaiting(QThread::objectName(), "accessor");
                accessor.lock();
                wlInstance->removeWaiting(QThread::objectName(), "accessor");
            }
        }

        first = false;

        mutex.unlock();
    }

    virtual void unlockWriter() {
        wlInstance->addWaiting(QThread::objectName(), "mutex");
        mutex.lock();
        wlInstance->removeWaiting(QThread::objectName(), "mutex");

        if (waitingIds.size() > 0) {
            toRealeaseId = waitingIds[0];
            waitingIds.erase(0);
            accessor.unlock();
        } else {
            first = true;
        }

        mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADINGMUT_H
