#include "owaitcondition.h"

OWaitCondition::OWaitCondition() {}

bool OWaitCondition::wait(OMutex *lockedMutex) {
    return cond.wait(lockedMutex->getMutex());
}

void OWaitCondition::wakeAll() {
    cond.wakeAll();
}

void OWaitCondition::wakeOne() {
    cond.wakeOne();
}
