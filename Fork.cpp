#include "Fork.h"

void Fork::pickUp() {
    mtx.lock();
}

void Fork::putDown() {
    mtx.unlock();
}
