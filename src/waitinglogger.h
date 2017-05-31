#ifndef WAITINGLOGGER_H
#define WAITINGLOGGER_H

#include <QStringList>
#include <QList>

class WaitingQueue
{
    QString name;
    QStringList threadNames;
};

class WaitingLogger
{
public:

    /**
     * @brief getInstance Returns the singleton
     * @return the only instance of WaitingLogger
     * This logger has to be a singleton, accessed by every synchronization
     * object
     */
    static WaitingLogger *getInstance();


    void addWaiting(const QString& threadName,const QString& objectName);

    void removeWaiting(const QString& threadName,const QString& objectName);



protected:
    WaitingLogger();

    QList<WaitingQueue *> getQueues() const;

    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    virtual void updateView();

    QList<WaitingQueue *> queues;

};

class ReadWriteLogger : public WaitingLogger
{
    friend WaitingLogger;

public:

    QStringList getResourceAccesses() const;

    void addResourceAccess(const QString &threadName);
    void removeResourceAccess(const QString &threadName);

protected:

    ReadWriteLogger();

    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    virtual void updateView();

    QStringList resourceAccesses;

};

#endif // WAITINGLOGGER_H
