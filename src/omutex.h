#ifndef PMUTEX_H
#define PMUTEX_H

#include <QMutex>

class OMutex
{

private:
    QMutex mutex;
    bool locked;

public:
    OMutex();

    void lock();

    bool tryLock();

    void unlock();

    bool getState();

    OMutex* getMutex();
};

#endif // PMUTEX_H
