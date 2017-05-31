#include "omutex.h"

OMutex::OMutex()
{
    mutex = new QMutex();
    locked = false;
}

void OMutex::lock()
{
    mutex.lock();

    locked = true;

}

bool OMutex::tryLock()
{
    if(mutex.tryLock())
    {
        locked = true;
        return true;
    }

    return false;
}


void OMutex::unlock()
{
    locked = false;

    mutex.unlock();
}

bool OMutex::getState()
{
    return locked;
}

QMutex OMutex::getMutex()
{
    return mutex;
}
