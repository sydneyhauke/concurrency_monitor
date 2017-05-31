#include "osemaphore.h"

OSemaphore::OSemaphore(int n) : sem(n), v(n) {}

void OSemaphore::acquire() {
    v--;
    sem.acquire();
}

void OSemaphore::release() {
    v++;
    sem.release();
}

bool OSemaphore::tryAcquire() {
    bool hasAcquired = sem.tryAcquire();

    if(hasAcquired) {
        v--;
    }

    return hasAcquired;
}

int OSemaphore::semVal() {
    return v;
}
