/**
  \file readerwriterequalmut.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe implémentant la priorité égale avec des mutex
*/

#ifndef READERWRITEREQUALMUT_H
#define READERWRITEREQUALMUT_H

#include "ireaderwriter.h"
#include "omutex.h"
#include "waitinglogger.h"

#include <QVector>
#include <algorithm>
#include <QThread>

class readerwriterequalmut : public IReaderWriter
{
protected:
    OMutex mutex;
    OMutex accessor;
    int nbReaders;
    QVector<int> waitingIds;
    int currentId;
    int toRealeaseId;
    bool readerAccessing;
    bool first;

    WaitingLogger *wlInstance;

public:

    readerwriterequalmut() :
        mutex(),
        accessor(),
        nbReaders(0),
        currentId(0),
        toRealeaseId(-1),
        readerAccessing(false),
        first(true)
    {
        wlInstance = WaitingLogger::getInstance();
    }

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;

        if (!first && !readerAccessing) {
            int id = currentId++;
            waitingIds.push_back(id);

            while (!first && !readerAccessing && (toRealeaseId!= id)) {
                accessor.lock();
            }
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
            waitingIds.remove(0);
            accessor.unlock();
        } else if (nbReaders == 0) {
            readerAccessing = false;
        } else {
            first = true;
        }

        mutex.unlock();
    }

    virtual void lockWriter() {
        mutex.lock();

        if (!first) {
            int id = currentId++;
            waitingIds.push_back(id);

            while (toRealeaseId!= id) {
                accessor.lock();
            }
        }

        first = false;

        mutex.unlock();
        wlInstance->addWaiting(QThread::currentThread()->objectName(), "writer");
        accessor.lock();
        wlInstance->removeWaiting(QThread::currentThread()->objectName(), "writer");
    }

    virtual void unlockWriter() {
        mutex.lock();

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

#endif // READERWRITEREQUALMUT_H
