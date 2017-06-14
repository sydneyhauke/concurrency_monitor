
#include <QApplication>
#include <QKeyEvent>
#include "synchrocontroller.h"

int main(int argc, char *argv[])
{

    // Create the resource manager object

    // Create the threads

    // Start the threads

    bool continuing = true;

    while (continuing) {
        // Wait for a key press
        QKeyEvent *event;

        // If key is <enter>
        if(event->key() == Qt::Key_Enter){
            SynchroController::getInstance()->resume();
        }

        if(event->key() == Qt::Key_Escape){
            continuing = false;
        }
    }

    // Kill the threads

    return 0;



    // Pour ceux qui voudraient développer une version graphique
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
