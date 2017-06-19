/**
  \file omutex.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Decorator declaration of a Mutex. Adds some functionnalities, like
  getting the state of the mutex instance.
*/

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

    QMutex* getMutex();
};

#endif // PMUTEX_H
