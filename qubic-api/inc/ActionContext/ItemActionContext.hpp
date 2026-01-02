#include <jni.h>

namespace Qubic {
    struct ItemActionContext final {
    public:
        jobject player;       /* net.minecraft.world.entity.player.Player */
        jobject world;        /* net.minecraft.world.level.Level */
        jobject item_stack;   /* net.minecraft.world.item.ItemStack */
        jobject block_pos;    /* net.minecraft.core.BlockPos (nullable - only for block interactions) */
        jobject entity;       /* net.minecraft.world.entity.Entity (nullable - only for entity interactions) */
        
        JNIEnv* env;          
        
        int face;             /* block face (0-5, only valid for block interactions) */
        
        /* callback should set these result interfaces */
        bool success;
        bool consume_item;
    };
    using IAC = Qubic::ItemActionContext;
}