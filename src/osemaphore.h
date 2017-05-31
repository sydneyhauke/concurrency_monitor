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
