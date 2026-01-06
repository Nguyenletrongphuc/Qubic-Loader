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
#include <unordered_map>
#include <map>
#include <set>

namespace fs = std::filesystem;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef GetMessage
#undef GetMessage
#endif
#ifdef PostMessage
#undef PostMessage
#endif
#endif // _WIN32

#ifdef _WIN32
#define MOD_EXPORT __declspec(dllexport)
#else
#define MOD_EXPORT __attribute__((visibility("default")))
#endif

namespace Qubic {
    class ModLoader;
    struct ItemDescriptor;
    
    struct PlayerJoinEvent;
    struct PlayerTickEvent;
    struct KeyInputEvent;

    struct Vec3 final {
    public:
        float x, y, z;
    };

    struct PendingItemCallback final {
    public:
        std::string full_item_id;  // "modid:itemid"
        ItemDescriptor* descriptor;
    };

    /* mod state, the base of everything */
    /* mod loader passes basic mod state with jvmti, jvm, and jni instances */
    /* your mod fills in the missing pieces like the "mod_id" field then returns it */
    /* very basic architechure, and I think it works quite well*/
    struct ModState final {
    public:
        const char* mod_id;
        void* data_ptr;

        /* vector to store pending item callbacks */
        std::vector<PendingItemCallback> pending_callbacks;
        
        JavaVM* GetJVM()     const { return this->jvm_ptr;   }
        JNIEnv* GetJNI()     const { return this->jni_env;   }
        jvmtiEnv* GetJVMTI() const { return this->jvmti_env; }
    private:
        friend class ModLoader;
        JavaVM* jvm_ptr;
        JNIEnv* jni_env;
        jvmtiEnv* jvmti_env;
    };

    struct BaseMod {
    public:
        virtual void init(Qubic::ModState* state) = 0;
        virtual void on_tick(Qubic::ModState* state) { }

        virtual void OnPlayerTick(PlayerTickEvent* e) { }
        virtual void OnPlayerJoin(PlayerJoinEvent* e) { }
        virtual void OnKeyInput(KeyInputEvent* e) { }
        
        virtual ~BaseMod() = default;
    };
}

extern "C" {
    MOD_EXPORT Qubic::ModState* mod_load(Qubic::ModState* state);
}

#endif