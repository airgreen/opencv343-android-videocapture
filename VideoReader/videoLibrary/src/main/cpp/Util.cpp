#include "Util.h"

static JavaVM *savedVm = nullptr;

void setVM(JavaVM *vm) {
    savedVm = vm;
}

JavaVM *getVM() {
    return savedVm;
}