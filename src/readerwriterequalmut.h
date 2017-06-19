#ifndef READERWRITEREQUALMUT_H
#define READERWRITEREQUALMUT_H

#include "ireaderwriter.h"
#include "omutex.h"
#include <vector>

class readerwriterequalmut : public IReaderWriter
{
protected:
    OMutex mutex;
    OMutex writer;
    int nbReaders;
    std::vector<int> waitingIds;
    int currentId;
    int toRealeaseId;

public:

    readerwriterequalmut() :
        mutex(1),
        writer(1),
        nbReaders(0),
        currentId(0),
        toRealeaseId(-1)
    {}

    virtual void lockReader() {
        mutex.lock();
        nbReaders++;

        if (nbReaders > 0) {

            int id = currentId++;
            waitingIds.push_back(id);

            while (nbReaders > 0 && (toRealeaseId!= id)) {
                writer.unlock();
            }
        }
        mutex.unlock();
    }

    virtual void unlockReader() {

        mutex.lock();
        nbReaders--;

        if (waitingIds.size() > 0) {
            toRealeaseId = waitingIds[0];
            waitingIds.erase(0);
            writer.unlock();
        }

        mutex.unlock();
    }

    virtual void lockWriter() {
        writer.lock();
    }

    virtual void unlockWriter() {
        writer.unlock();
    }


};

#endif // READERWRITEREQUALMUT_H
