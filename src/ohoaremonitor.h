/**
  \file ohoaremonitor.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Decorator declaration of a HoareMonitor.
*/

#ifndef OHOAREMONITOR_H
#define OHOAREMONITOR_H

#include "osemaphore.h"
class OHoareMonitor
{
protected:
    OHoareMonitor();
    class Condition
    {
        friend OHoareMonitor;
    public:
        Condition();
    private :
        OSemaphore waitingOSem;
        int nbWaiting;
    };

public:

    /**
     * This function has to be called at the beginning of each function being
     * an entry point to the monitor.
     */
    void monitorIn();

    /**
     * This function has to be called at the end of each function being
     * an entry point to the monitor.
     */
    void monitorOut();

    /**
     * This function implements the waiting on a condition, as defined by Hoare.
     * When the thread is waken by a signal, it continues with the mutual
     * exclusion.
     */
    void wait(Condition &cond);

    /**
     * This function implements the signaling of a condition, as defined by
     * Hoare. If no thread is waiting for the condition, then nothing happens,
     * but if there is one the thread calling signal is suspended, waiting for
     * the other one to finish.
     */
    void signal(Condition &cond);

private:

    //! Mutex for the protecting the entire monitor
    OSemaphore monitorMutex;
    //! Blocking semaphore for the thread doing a signal(condition)
    OSemaphore monitorSignale;
    //! Number of threads waiting on the semaphore monitorSignale
    int monitorNbSignale;

};

#endif // OHOAREMONITOR_H
