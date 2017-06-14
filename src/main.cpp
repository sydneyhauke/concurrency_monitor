
#include <QApplication>
#include <QThread>
#include <iostream>

#include "synchrocontroller.h"
#include "readerwriterprioreadermut.h"
#include "ireaderwriter.h"

#define NB_READERS 5
#define NB_WRITERS 3

readerwriterprioreadermut resource();

class Writer : public QThread {
private :
    IReaderWriter *_resource;

public:
    Writer(IReaderWriter *resource) : _resource(resource) {}

    void run() Q_DECL_OVERRIDE {
        while(1) {
            _resource->lockWriter();
            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockWriter();
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
            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));

            _resource->unlockReader();
            usleep((int)((float)600000*rand()/(RAND_MAX+1.0)));
        }
    }
};

int main(int argc, char *argv[])
{
    int input = 0;

    Reader* readerThreads[NB_READERS];
    Writer* writerThreads[NB_WRITERS];

    // Create the resource manager object
    IReaderWriter *resource = new readerwriterprioreadermut();

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
        std::cout << "Push on Enter to continue ..." << std::endl;
        std::cin >> input;
        std::cout << "value of input : " << input << std::endl;
        // If key was <Enter>
        if (input == 0) {
            std::cout << "Hello I'm in the while loop" << std::endl;
            //SynchroController::getInstance()->resume();
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

    for(size_t i = 0; i < NB_WRITERS; i++) {
        delete writerThreads[i];
    }

    delete resource;

    std::cout << "Hello I'm finishing the program" << std::endl;

    return 0;



    // Pour ceux qui voudraient développer une version graphique
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
