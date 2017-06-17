#include "waitinglogger.h"
#include <iostream>

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
    return waitingQueues;
}



/**
 * @brief WaitingLogger::addWaiting Adds a thread to a waiting queue. If the waiting doesn't already
 * exist, it is created.
 * @param threadName A thread name to add in a waiting queue
 * @param objectName A name of the waiting queue
 */
void WaitingLogger::addWaiting(const QString& threadName, const QString& objectName) {
    bool found = false;
    WaitingQueue *wq;

    // Check if corresponding waiting queue already exists
    for(QList<WaitingQueue *>::iterator it = waitingQueues.begin(); it != waitingQueues.end(); it++) {
        if((*it)->name == objectName) {
            found = true;
            wq = (*it);
            break;
        }
    }

    if(!found) {
        wq = new WaitingQueue();
        wq->name = threadName;
    }

    mutex.lock();
    wq->threadNames.push_back(threadName);
    mutex.unlock();
}



/**
 * @brief WaitingLogger::removeWaiting Removes a thread from a waiting queue. If a waiting queue
 * is empty, it is not removed.
 * @param threadName A thread name to add in a waiting queue
 * @param objectName A name of the waiting queue
 */
void WaitingLogger::removeWaiting(const QString& threadName, const QString& objectName) {
    bool found = false;
    WaitingQueue *wq;

    // Check if waiting queue exists
    for(QList<WaitingQueue *>::iterator it = waitingQueues.begin(); it != waitingQueues.end(); it++) {
        if((*it)->name == objectName) {
            found = true;
            wq = (*it);
            break;
        }
    }

    if(found) {
        mutex.lock();
        wq->threadNames.removeAll(threadName);
        mutex.unlock();
    }
}



void WaitingLogger::updateView()
{
    for(QList<WaitingQueue *>::iterator itQueue = waitingQueues.begin(); itQueue != waitingQueues.end();itQueue++) {
        std::cout << (*itQueue)->name.toStdString() << "       ";
        for(QStringList::iterator itName = (*itQueue)->threadNames.begin(); itName != (*itQueue)->threadNames.end(); itName++) {
            std::cout << (*itName).toStdString() << ", "; // TODO : ne pas afficher de séparateur après le dernier nom
        }
        std::cout << std::endl;
    }
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
    mutex.lock();
    resourceAccesses.push_back(threadName);
    mutex.unlock();
}



void ReadWriteLogger::removeResourceAccess(const QString &threadName)
{
    mutex.lock();
    resourceAccesses.removeAll(threadName);
    mutex.unlock();
}




void ReadWriteLogger::updateView()
{
    std::cout << "In resource : ";
    for(QStringList::iterator it = resourceAccesses.begin(); it != resourceAccesses.end(); it++) {
        std::cout << it->toStdString() << ", "; // TODO : ne pas afficher de séparateur après le dernier nom
    }
    std::cout << std::endl;
}
