#ifndef Qubic_HPP
#define Qubic_HPP

#include <jni.h>
#include <jvmti.h>

#include <cstdint>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <filesystem>
namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
#define MOD_EXPORT __declspec(dllexport)
#else
#define MOD_EXPORT __attribute__((visibility("default")))
#endif

namespace Qubic {
    class ModLoader;

    /* mod state, the base of everything */
    /* mod loader passes basic mod state with jvmti, jvm, and jni instances */
    /* your mod fills in the missing pieces like the "mod_id" field then returns it */
    /* very basic architechure, and I think it works quite well*/
    struct ModState final {
    public:
        const char* mod_id;
        void* data_ptr;
        
        JavaVM* GetJVM()     const { return this->jvm_ptr;   }
        JNIEnv* GetJNI()     const { return this->jni_env;   }
        jvmtiEnv* GetJVMTI() const { return this->jvmti_env; }
    private:
        friend class ModLoader;
        JavaVM* jvm_ptr;
        JNIEnv* jni_env;
        jvmtiEnv* jvmti_env;
    };

    /* base mod, pretty cool.. you should inherit from it cause it gives your mod structure */
    /* .. anyone who thought they could use qubic to escape 'java class hell' will be disappointed .. */
    /* (jk) you can write your mod code however you want, as long as the mod state has methods for the mod loader to execute */
    struct BaseMod {
    public:
        virtual void init(Qubic::ModState* state) = 0;
        virtual void on_tick(Qubic::ModState* state) {}
        
        virtual ~BaseMod() = default;
    };
}

extern "C" {
    MOD_EXPORT Qubic::ModState* mod_load(Qubic::ModState* state);
}

#endif