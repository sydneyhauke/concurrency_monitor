/**
  \file main.cpp
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Classe main du projet. Entre autres, elle :
  *              - Crée des threads rédacteurs et lecteurs
  *              - Démarre les threads à des instants aléatoires
  *              - Crée une ressource
  *              - Permet d'avancer ou arrêter le programme
  *
*/

#include <QApplication>
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>

#include "synchrocontroller.h"

#include "readerwriterequalcond.h"
#include "readerwriterequalhoare.h"
#include "readerwriterequalmut.h"
#include "readerwriterequalsem.h"

#include "readerwriterprioreadercond.h"
#include "readerwriterprioreaderhoare.h"
#include "readerwriterprioreadermut.h"
#include "readerwriterprioreadersem.h"

#include "readerwriterprioreadingcond.h"
#include "readerwriterprioreadinghoare.h"
#include "readerwriterprioreadingmut.h"
#include "readerwriterprioreadingsem.h"

#include "readerwriterpriowritercond.h"
#include "readerwriterpriowriterhoare.h"
#include "readerwriterpriowritermut.h"
#include "readerwriterpriowritersem.h"

#include "ireaderwriter.h"
#include "waitinglogger.h"

#define NB_READERS 2
#define NB_WRITERS 3

/**
 * @brief The Writer class
 *
 * Simple thread class that acts as a writer on a resource. Accesses and exits resources
 * at randome times.
 */
class Writer : public QThread {
private :
    IReaderWriter *_resource;
    bool _shouldTerminate;
    static QSemaphore sem;

public:
    Writer(IReaderWriter *resource) : _resource(resource), _shouldTerminate(false) {}
    ~Writer() {
        _shouldTerminate = true;
        sem.release();
    }

    void run() Q_DECL_OVERRIDE {
        while(1) {
            sem.acquire();
            if(_shouldTerminate) {
                break;
            }
            sem.release();

            _resource->lockWriter(); // Access resource
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->addResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockWriter(); // Exits resource
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->removeResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));
        }
    }
};

QSemaphore Writer::sem(1);

/**
 * @brief The Reader class
 *
 * Simple thread class that acts as a reader on a resource. Accesses and exits resources
 * at randome times.
 */
class Reader : public QThread {
private:
    IReaderWriter *_resource;
    bool _shouldTerminate;
    static QSemaphore sem;

public:
    Reader(IReaderWriter *resource) : _resource(resource), _shouldTerminate(false) {}
    ~Reader() {
        _shouldTerminate = true;
        sem.release();
    }

    void run() Q_DECL_OVERRIDE {
        while(1) {
            sem.acquire();
            if(_shouldTerminate) {
                break;
            }
            sem.release();

            _resource->lockReader(); // Access resource
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->addResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockReader(); // Exits resource
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->removeResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));
        }
    }
};

QSemaphore Reader::sem(1);

int main(int argc, char *argv[])
{
    Reader* readerThreads[NB_READERS];
    Writer* writerThreads[NB_WRITERS];

    // Create the resource manager object
    // =====================================================
    // NOTE : CHANGE READ_WRITE RESOURCE IMPLEMENTATION HERE
    // =====================================================
    IReaderWriter *resource = new readerwriterprioreaderSem();

    // Create the threads
    for(size_t i = 0; i < NB_READERS; i++) {
        readerThreads[i] = new Reader(resource);

        QString threadName = QString("Reader%1").arg(i);
        readerThreads[i]->setObjectName(threadName);
    }

    for(size_t i = 0; i < NB_WRITERS; i++) {
        writerThreads[i] = new Writer(resource);

        QString threadName = QString("Writer%1").arg(i);
        writerThreads[i]->setObjectName(threadName);
    }

    // Start the threads
    for(size_t i = 0; i < NB_READERS; i++) {
        readerThreads[i]->start();
    }

    for(size_t i = 0; i < NB_WRITERS; i++) {
        writerThreads[i]->start();
    }

    bool continuing = true;
    int input;

    while (continuing) {
        // Wait for a key press
        std::cout << "Push on <Enter> to continue or <esc> to stop ..." << std::endl;

        input = std::cin.get();

        int c = input;
        while(c != '\n') {
            c = std::cin.get();
        }

        // If key was <Enter>
        if (input == '\n') {
            SynchroController::getInstance()->resume();
        }
        // If key was <esc>
        else if (input == 27) {
            continuing = false;
        }

    }

    /* Destroy a single thread at a time */
    for(size_t i = 0; i < NB_WRITERS; i++) {
        delete writerThreads[i];
    }

    for(size_t i = 0; i < NB_READERS; i++) {
        delete readerThreads[i];
    }

    std::cout << "Program finised" << std::endl;

    return 0;

    // Pour ceux qui voudraient développer une version graphique
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
