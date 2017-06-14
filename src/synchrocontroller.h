#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H

#include <QSemaphore>

class SynchroController
{
private:
    QSemaphore barrier;

    SynchroController();
public:
    static SynchroController *getInstance();

    void pause();

    void resume();
};

#endif // SYNCHROCONTROLLER_H
