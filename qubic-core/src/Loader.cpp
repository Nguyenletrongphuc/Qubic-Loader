#include <qubic-api/inc/Qubic.hpp>
#include <src/core/Binary.h>
#include <src/Loader.hpp>

bool Qubic::ModLoader::LoadMod(const fs::path& Directory) {
    void* Handle = LoadBin(Directory.string().c_str());
    if (!Handle) return false;

    ModLoadFunc mod_load = (ModLoadFunc)GET_FUNC(Handle, "mod_load");
    if (!mod_load) {
        CloseBin(Handle);
        return false;
    }

    Qubic::ModState* State = new Qubic::ModState();
    State->jvm_ptr = jvm_ptr;
    State->jni_env = jni_env;
    State->jvmti_env = jvmti_env;
    State->mod_id = nullptr; /* set by mod */
    State->data_ptr = nullptr;

    Qubic::ModState* Result = mod_load(State);
    if (!Result || !Result->mod_id) {
        delete State;
        CloseBin(Handle);
        return false;
    }
    
    for (const auto& Mod : this->ModVector) {
        if (strcmp(Mod.State.mod_id, Result->mod_id) == 0) {
            printf("[Qubic] ERROR: Duplicate mod_id detected: %s\n", Result->mod_id);
            delete State;
            CloseBin(Handle);
            return false;
        }
    }
    
    this->ModVector.push_back({Result->mod_id, *State, Handle});
    printf("[Qubic] DEBUG: Loaded ModId: %s\n", Result->mod_id);
    return true;
}