/**
  \file ireaderwriter.h
  \author Sydney Hauke, Thuy-My Tran, Yosra Harbaoui et Denise Gemesio
  \date 19.06.2017
  \brief Abstract class declaration. Acts as an interface for all ReaderWriter
  implementations. Provides the base methods lockReader(), unlockReader(),
  lockWriter() and unlockWriter().
*/

#ifndef IREADERWRITER_H
#define IREADERWRITER_H

class IReaderWriter {
public:
    virtual void lockReader() = 0;
    virtual void unlockReader() = 0;
    virtual void lockWriter() = 0;
    virtual void unlockWriter() = 0;
};

#endif // IREADERWRITER_H
