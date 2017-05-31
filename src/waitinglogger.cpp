#include "waitinglogger.h"

WaitingLogger::WaitingLogger()
{

}

WaitingLogger *WaitingLogger::getInstance()
{
    static WaitingLogger *instance = new ReadWriteLogger();
    return instance;
}

QList<WaitingQueue *> WaitingLogger::getQueues() const
{
    return queues;
}

void WaitingLogger::updateView()
{

}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger()
{

}

void ReadWriteLogger::addResourceAccess(const QString &threadName)
{

}

void ReadWriteLogger::removeResourceAccess(const QString &threadName)
{

}


void ReadWriteLogger::updateView()
{

}
