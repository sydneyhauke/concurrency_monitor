#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H


class SynchroController
{
    SynchroController();
public:
    static SynchroController *getInstance();

    void pause();

    void resume();
};

#endif // SYNCHROCONTROLLER_H
