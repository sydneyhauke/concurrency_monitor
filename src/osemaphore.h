#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H


class OSemaphore
{
public:
    OSemaphore(int n = 0);

    void acquire();

    void release();

    bool tryAcquire();
};

#endif // PSEMAPHORE_H
