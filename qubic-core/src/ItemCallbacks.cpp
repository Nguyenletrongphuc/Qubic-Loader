#include <qubic-api/inc/Registry/ItemRegistry.hpp>
#include <qubic-api/inc/ActionContext/ItemActionContext.hpp>
#include <src/core/Binary.h>

#include <unordered_map>
#include <string>
#include <jni.h>

/* gmap for all the item callbacks */
std::unordered_map<std::string, Qubic::ItemDescriptor> g_ItemCallbacks;

extern "C" {
    DLL_EXPORT JNIEXPORT jboolean JNICALL Java_QubicItemHandler_OnItemUse(JNIEnv* env, jclass clazz, jstring jitem_id, jobject jplayer, jobject jworld, jobject jstack) {
        const char* item_id = env->GetStringUTFChars(jitem_id, nullptr);
        
        printf("[Qubic] OnItemUse called for: %s\n", item_id);
        
        /* find callback in map */
        auto it = g_ItemCallbacks.find(item_id);
        if (it == g_ItemCallbacks.end()) {
            printf("[Qubic] No callback found for: %s\n", item_id);
            env->ReleaseStringUTFChars(jitem_id, item_id);
            return false;
        }
        
        if (!it->second.on_use) {
            printf("[Qubic] Callback is null for: %s\n", item_id);
            env->ReleaseStringUTFChars(jitem_id, item_id);
            return false;
        }
        
        Qubic::ItemActionContext ctx;
        ctx.player = jplayer;
        ctx.world = jworld;
        ctx.item_stack = jstack;
        ctx.block_pos = nullptr;
        ctx.entity = nullptr;
        ctx.env = env;
        ctx.face = -1;
        ctx.success = false;
        ctx.consume_item = false;
        
        printf("[Qubic] Calling callback for: %s\n", item_id);
        it->second.on_use(&ctx);
        
        env->ReleaseStringUTFChars(jitem_id, item_id);
        
        printf("[Qubic] Callback result: %d\n", ctx.success);
        return ctx.success;
    }
}