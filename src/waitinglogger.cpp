#include "waitinglogger.h"
#include "synchrocontroller.h"
#include <iostream>

WaitingLogger::WaitingLogger() {}

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
 * @brief WaitingLogger::addWaiting Adds a thread to a waiting queue. If the waiting queue doesn't already
 * exist, it is created.
 * @param threadName A thread name to add in a waiting queue
 * @param objectName A name of the waiting queue
 */
void WaitingLogger::addWaiting(const QString& threadName, const QString& objectName) {
    mutex.lock();
    bool found = false;
    WaitingQueue *wq;

    /* Check if corresponding waiting queue already exists.
     * NOTE : could use a HashMap instead. Optimisation from O(N) to O(1)
     */
    for(QList<WaitingQueue *>::iterator it = waitingQueues.begin(); it != waitingQueues.end(); it++) {
        if((*it)->name == objectName) {
            found = true;
            wq = (*it);
            break;
        }
    }

    if(!found) {
        wq = new WaitingQueue();
        wq->name = objectName;
        waitingQueues.push_back(wq);
    }

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
    mutex.lock();

    bool found = false;
    WaitingQueue *wq;


    /* Check if corresponding waiting queue already exists.
     * NOTE : could use a HashMap instead. Optimisation from O(N) to O(1)
     */
    for(QList<WaitingQueue *>::iterator it = waitingQueues.begin(); it != waitingQueues.end(); it++) {
        if((*it)->name == objectName) {
            found = true;
            wq = (*it);
            break;
        }
    }

    if(found) {
        wq->threadNames.removeAll(threadName);
    }

    mutex.unlock();
}

/**
 * @brief WaitingLogger::updateView Displays waiting queues and their threads that have been registered
 */
void WaitingLogger::updateView()
{
    for(QList<WaitingQueue *>::iterator itQueue = waitingQueues.begin(); itQueue != waitingQueues.end(); itQueue++) {
        std::cout << (*itQueue)->name.toStdString() << " : ";
        for(QStringList::iterator itName = (*itQueue)->threadNames.begin(); itName != (*itQueue)->threadNames.end(); itName++, std::cout << (itName == (*itQueue)->threadNames.end() ? "":", ")) {
            std::cout << (*itName).toStdString();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger() {}

void ReadWriteLogger::addResourceAccess(const QString &threadName)
{
    /* Let access threads in resource in a sequential manner.
     * But ! It still lets multiple readers to access it concurrently.
     * We only need mutual exclusion when updating the list of threads in
     * resource and also when updating the view.
     */
    mutex.lock();

    resourceAccesses.push_back(threadName);

    std::cout << "== Logger state ==" << std::endl;
    WaitingLogger::updateView();
    updateView();
    SynchroController::getInstance()->pause();

    mutex.unlock();
}

void ReadWriteLogger::removeResourceAccess(const QString &threadName)
{
    mutex.lock();
    resourceAccesses.removeAll(threadName);
    mutex.unlock();
}

/**
 * @brief ReadWriteLogger::updateView Displays all threads that are in resource
 */
void ReadWriteLogger::updateView()
{
    std::cout << "In resource : ";
    for(QStringList::iterator it = resourceAccesses.begin(); it != resourceAccesses.end(); it++, std::cout << (it == resourceAccesses.end() ? "":", ")) {
        std::cout << it->toStdString();
    }
    std::cout << std::endl << std::endl;
}
