#ifndef LOADER_HPP
#define LOADER_HPP

#include <qubic-api/inc/Qubic.hpp>
#include <src/core/Binary.h>

#include <filesystem>
#include <cstdint>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <memory>

namespace fs = std::filesystem;

#define SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

typedef Qubic::ModState* (*ModLoadFunc)(Qubic::ModState*);
extern std::unordered_map<std::string, Qubic::ItemDescriptor> g_ItemCallbacks;

namespace {
    struct LoadedMod final {
    public:
        const char* ModID;
        Qubic::ModState State;
        void* Handle;
        Qubic::BaseMod* ModInstance;
    };
}

namespace Qubic {
    class ModLoader final {
    private:
        JavaVM* jvm_ptr;
        JNIEnv* jni_env;
        jvmtiEnv* jvmti_env;
    public:
        std::vector<LoadedMod> ModVector;
        
        ModLoader(JavaVM* jvm, JNIEnv* env, jvmtiEnv* jvmti) : jvm_ptr(jvm), jni_env(env), jvmti_env(jvmti) {}
        bool LoadMod(const fs::path& Directory);

        ~ModLoader() = default;
    };
}

#endif 