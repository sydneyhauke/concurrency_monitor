
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = readwritemonitoring
TEMPLATE = app
CONFIG += console

INCLUDEPATH += src


SOURCES += src/main.cpp\
    src/waitinglogger.cpp \
    src/synchrocontroller.cpp \
    src/omutex.cpp \
    src/osemaphore.cpp \
    src/owaitcondition.cpp \
    src/ohoaremonitor.cpp

HEADERS  +=  \
    src/waitinglogger.h \
    src/synchrocontroller.h \
    src/omutex.h \
    src/osemaphore.h \
    src/owaitcondition.h \
    src/ohoaremonitor.h \
    src/ireaderwriter.h \
    src/readerwriterprioreadersem.h \
    src/readerwriterpriowritersem.h \
    src/readerwriterequalsem.h \
    src/readerwriterpriowritermut.h \
    src/readerwriterequalmut.h \
    src/readerwriterprioreadercond.h \
    src/readerwriterpriowritercond.h \
    src/readerwriterequalcond.h \
    src/readerwriterprioreaderhoare.h \
    src/readerwriterpriowriterhoare.h \
    src/readerwriterequalhoare.h \
    src/readerwriterprioreadermut.h \
    src/readerwriterequalcond.h
