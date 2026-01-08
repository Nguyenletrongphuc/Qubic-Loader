#include <jni.h>
#include <jvmti.h>
#include <cstring>
#include <vector>
#include <string>
#include <cstdio>
#include <thread>
#include <chrono>
#include <filesystem>

#include <qubic-api/inc/Event/PlayerJoinEvent.hpp>
#include <qubic-api/inc/Event/PlayerTickEvent.hpp>

#include <qubic-api/inc/Event/KeyInputEvent.hpp>
#include <qubic-api/inc/Event/MouseInputEvent.hpp>
#include <qubic-api/inc/Event/MouseScrollInputEvent.hpp>

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
        printf("[Qubic] RegisterModItems called\n");
        printf("[Qubic] Registry initialization in progress\n");
        
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

    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnPlayerJoin(JNIEnv* env, jclass clazz, jobject player, jobject world) {
        if (!gModLoader) {
            printf("[Qubic] ERROR: ModLoader not initialized!\n");
            return;
        }
        
        printf("[Qubic] Player joined event triggered\n");
        Qubic::PlayerJoinEvent event;
        event.player = player;
        event.world = world;
        event.env = env;
        
        /* dispatch player join event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->OnPlayerJoin(&event);
        }
    }
    
    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnPlayerTick(JNIEnv* env, jclass clazz, jobject player, jobject world) {
        if (!gModLoader) return;
        
        Qubic::PlayerTickEvent event;
        event.player = player;
        event.world = world;
        event.env = env;
        
        /* dispatch on tick event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->OnPlayerTick(&event);
        }
    }
    
    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnServerTick(JNIEnv* env, jclass clazz) {
        if (!gModLoader) return;
        
        /* dispatch on sever tick event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->on_tick(&mod.State);
        }
    }

    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnKeyInput(JNIEnv* env, jclass clazz, jint key, jint scancode, jint action, jint mods) {
        if (!gModLoader) return;
        
        Qubic::KeyInputEvent event;
        event.key = key;
        event.scancode = scancode;
        event.action = action;
        event.mods = mods;
        event.env = env;

        /* dispatch key input event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->OnKeyInput(&event);
        }
    }

    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnMouseButton(JNIEnv* env, jclass clazz, jint button, jint action, jint mods) {
        if (!gModLoader) return;
        
        Qubic::MouseInputEvent event;
        event.button = button;
        event.action = action;
        event.mods = mods;
        event.env = env;
    
        /* dispatch on mouse button event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->OnMouseButton(&event);
        }
    }

    DLL_EXPORT JNIEXPORT void JNICALL Java_QubicNative_OnMouseScroll(JNIEnv* env, jclass clazz, jdouble horizontal, jdouble vertical) {
        if (!gModLoader) return;
        
        Qubic::MouseScrollInputEvent event;
        event.horizontal = horizontal;
        event.vertical = vertical;
        event.env = env;
    
        /* dispatch on mouse scroll input event to all mods */
        for (auto& mod : gModLoader->ModVector) {
            if (mod.ModInstance)
                mod.ModInstance->OnMouseScroll(&event);
        }
    }
}
