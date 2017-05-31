#ifndef PMUTEX_H
#define PMUTEX_H

#include <QMutex>
class OMutex
{

private:
    QMutex mutex;
    bool stopped;

public:
    OMutex();

    void lock();

    bool tryLock();

    void unlock();
};

#endif // PMUTEX_H
