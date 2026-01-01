#include <jni.h>
#include <jvmti.h>
#include <cstring>
#include <vector>
#include <string>
#include <cstdio>
#include <thread>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

jvmtiEnv* jvmti = nullptr;
JavaVM* jvm = nullptr;
JNIEnv* genv = nullptr;

#include <src/Loader.hpp>
#include <src/core/Binary.h>

Qubic::ModLoader* gModLoader = nullptr;

extern "C" {
    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_Init(JNIEnv* env, jclass clazz) {
        printf("[Qubic] DEBUG: Initializing Qubic Agent\n");

        env->GetJavaVM(&jvm);
        jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_2);
        
        if (jvmti == nullptr) {
            printf("[Qubic] ERROR: Failed to get JVMTI\n");
            return;
        }
        genv = env;
        
        printf("[Qubic] Agent initialized successfully\n");
    }

    /* this gets called during BuiltInRegistries.<clinit> (best timing to load the mods in YASSSSS) */
    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_RegisterModItems(JNIEnv* env, jclass clazz) {
        printf("[Qubic] ========================================\n");
        printf("[Qubic] RegisterModItems called\n");
        printf("[Qubic] Registry initialization in progress\n");
        printf("[Qubic] ========================================\n");
        
        if (jvm == nullptr || genv == nullptr || jvmti == nullptr) {
            printf("[Qubic] ERROR: JVM not initialized!\n");
            return;
        }
        
        gModLoader = new Qubic::ModLoader(jvm, genv, jvmti);
        fs::path ModFolder = fs::current_path() / "mods";

        if (!fs::exists(ModFolder)) {
            printf("[Qubic] DEBUG: Creating mods folder: %s\n", ModFolder.string().c_str());
            fs::create_directories(ModFolder);
        }

        printf("[Qubic] Scanning mods folder: %s\n", ModFolder.string().c_str());
        
        int ModCount = 0;
        for (const auto& Entry : fs::directory_iterator(ModFolder)) {
            if (Entry.is_regular_file()) {
                std::string Ext = Entry.path().extension().string();
                
    #ifdef _WIN32
                if (Ext == ".dll")
    #else
                if (Ext == ".so" || Ext == ".dylib")
    #endif
                {
                    printf("[Qubic] Loading mod: %s\n", Entry.path().filename().string().c_str());
                    bool Success = gModLoader->LoadMod(Entry.path());
                    if (Success) {
                        ModCount++;
                        printf("[Qubic] Mod loaded successfully!\n");
                    } else {
                        printf("[Qubic] CRITICAL ERROR: Failed to load mod!\n");
                    }
                }
            }
        }
        
        printf("[Qubic] Loaded %d mod(s)\n", ModCount);
        printf("[Qubic] All items registered!\n");
    }
}
