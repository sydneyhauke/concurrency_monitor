/**
  \file osemaphore.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Decorator declaration of a Semaphore. Adds some functionnalities, like
  getting the state of the semaphore instance.
*/

#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H

#include <QSemaphore>

class OSemaphore
{
private:
    QSemaphore sem;
    int v;

public:
    OSemaphore(int n = 0);

    void acquire();

    void release();

    bool tryAcquire();

    int semVal();
};

#endif // PSEMAPHORE_H
