#include "readerwriterprioreadermesa.h"

readerwriterprioreadermesa::readerwriterprioreadermesa() :
    mutex(1),
    writers(0),
    readers(0),
    writerAccessing(false)
{}

virtual void readerwriterprioreadermesa::lockReader() {

}
