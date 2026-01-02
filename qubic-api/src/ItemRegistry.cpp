#include <inc/Registry/ItemRegistry.hpp>

void Qubic::Item::SetItemDescriptor(Qubic::ItemDescriptor new_desc) {
    this->desc = new_desc;
}

[[nodiscard]] Qubic::Item* Qubic::RegisterItem(Qubic::ModState* state, const std::string& itemid, const Qubic::ItemDescriptor& desc) {
    printf("[Qubic] Registering item: %s:%s\n", state->mod_id, itemid.c_str());
    fflush(stdout);
    
    JNIEnv* env = state->GetJNI();
    if (!env) {
        printf("[Qubic] ERROR: JNIEnv is null!\n");
        fflush(stdout);
        return nullptr;
    }

    /* check if it has callbacks and register with qubic-core */
    bool HasCallbacks = (desc.on_use != nullptr);
    if (HasCallbacks) {
        std::string full_id = std::string(state->mod_id) + ":" + itemid;
        Qubic::PendingItemCallback pending;
        pending.full_item_id = full_id;
        pending.descriptor = new Qubic::ItemDescriptor(desc); // Allocate on heap
        state->pending_callbacks.push_back(pending);
        printf("[Qubic] Stored callback for: %s\n", full_id.c_str());
    }
    
    jclass RegistryClass = env->FindClass("QubicRegistry");
    if (!RegistryClass) {
        printf("[Qubic] ERROR: Could not find QubicRegistry class\n");
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        fflush(stdout);
        return nullptr;
    }
    
    jmethodID RegisterMethod = env->GetStaticMethodID(
        RegistryClass, 
        "registerItem", 
        "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;ZIZ)Z"
    );
    
    if (!RegisterMethod) {
        printf("[Qubic] ERROR: Could not find registerItem method\n");
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        fflush(stdout);
        return nullptr;
    }
    
    jstring JModID = env->NewStringUTF(state->mod_id);
    jstring Jid = env->NewStringUTF(itemid.c_str());
    jstring JDisplayName = env->NewStringUTF(desc.name.c_str());
    
    /* now we register */
    jboolean Success = env->CallStaticBooleanMethod(
        RegistryClass, 
        RegisterMethod, 
        JModID, 
        Jid, 
        desc.max_stack,
        JDisplayName,
        desc.fire_resistant,
        desc.durability,
        HasCallbacks
    );
    
    /* basic error check for safety measures */
    if (env->ExceptionCheck()) {
        printf("[Qubic] ERROR: Exception during item registration for mod: %s, id: %s\n", state->mod_id, itemid.c_str());
        env->ExceptionDescribe();
        env->ExceptionClear();
        fflush(stdout);
        return nullptr;
    }
    
    env->DeleteLocalRef(JModID);
    env->DeleteLocalRef(Jid);
    env->DeleteLocalRef(JDisplayName);
    env->DeleteLocalRef(RegistryClass);
    
    if (!Success) {
        printf("[Qubic] ERROR: Registration returned false\n");
        fflush(stdout);
        return nullptr;
    }
    
    printf("[Qubic] Successfully registered: %s:%s\n", state->mod_id, itemid.c_str());
    fflush(stdout);
    
    Qubic::Item* ItemPtr = new Qubic::Item();
    ItemPtr->SetID(itemid);
    ItemPtr->SetItemDescriptor(desc);
    
    return ItemPtr;
}