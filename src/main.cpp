
#include <QApplication>

#include "synchrocontroller.h"

int main(int argc, char *argv[])
{

    // Create the resource manager object

    // Create the threads

    // Start the threads

    bool continuing = true;

    while (continuing) {
        // Wait for a key press

        // If key is <enter>
        SynchroController::getInstance()->resume();

        // If key was <esc>
        continuing = false;
    }

    // Kill the threads

    return 0;



    // Pour ceux qui voudraient développer une version graphique
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
