
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = readwritemonitoring
TEMPLATE = app

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
    src/ohoaremonitor.h
