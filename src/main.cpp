
#include <QApplication>
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>

#include "synchrocontroller.h"
#include "readerwriterprioreadersem.h"
#include "ireaderwriter.h"
#include "waitinglogger.h"

#define NB_READERS 2
#define NB_WRITERS 3

class Writer : public QThread {
private :
    IReaderWriter *_resource;

public:
    Writer(IReaderWriter *resource) : _resource(resource) {}

    void run() Q_DECL_OVERRIDE {
        while(1) {
            _resource->lockWriter();
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->addResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockWriter();
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->removeResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));
        }
    }
};

class Reader : public QThread {
private:
    IReaderWriter *_resource;

public:
    Reader(IReaderWriter *resource) : _resource(resource){}

    void run() Q_DECL_OVERRIDE {
        while(1) {
            _resource->lockReader();
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->addResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockReader();
            ((ReadWriteLogger*)ReadWriteLogger::getInstance())->removeResourceAccess(this->objectName());

            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));
        }
    }
};

int main(int argc, char *argv[])
{
    int input;

    Reader* readerThreads[NB_READERS];
    Writer* writerThreads[NB_WRITERS];

    // Create the resource manager object
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

    // Kill the threads
    // Like, really ? killing ?...
    for(size_t i = 0; i < NB_WRITERS; i++) {
        writerThreads[i]->terminate();
    }

    for(size_t i = 0; i < NB_READERS; i++) {
        readerThreads[i]->terminate();
    }


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
