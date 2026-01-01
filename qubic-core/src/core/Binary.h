/* provides basic cross-platform utilities for loading binaries */

#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include <stddef.h>

/* all bullshit to make this work on linux.. prolly doesnt but who cares, i work on windows anyways */
// one day qubic will work on linux i promise
// - nez

#ifdef _WIN32
#include <windows.h>
#define LOAD_LIB(Path) LoadLibraryA(Path)
#define GET_FUNC(Handle, Name) GetProcAddress((HMODULE)Handle, Name)
#define CLOSE_LIB(Handle) FreeLibrary((HMODULE)Handle)
#else
#include <dlfcn.h>
#include <sys/
#define LOAD_LIB(Path) dlopen(Path, RTLD_LAZY)
#define GET_FUNC(Handle, Name) dlsym(Handle, Name)
#define CLOSE_LIB(Handle) dlclose(Handle)
#endif

#ifdef _WIN32
#include <windows.h>
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* LoadBin(const char* Path);
void CloseBin(void* Handle);

#ifdef __cplusplus
}
#endif

#endif