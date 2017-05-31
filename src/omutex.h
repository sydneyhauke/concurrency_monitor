#ifndef PMUTEX_H
#define PMUTEX_H


class OMutex
{
public:
    OMutex();

    void lock();

    bool tryLock();

    void unlock();
};

#endif // PMUTEX_H
