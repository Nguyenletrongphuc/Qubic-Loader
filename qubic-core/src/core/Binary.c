/* provides basic cross-platform utilities for loading binaries */

#include <src/core/Binary.h>

void* LoadBin(const char* Path) {
    void* Handle = LOAD_LIB(Path);
    if (!Handle) return NULL;

    return Handle;
}

void CloseBin(void* Handle) {
    if (!Handle) return;
    CLOSE_LIB(Handle);
}