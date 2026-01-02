#include <qubic-api/inc/Qubic.hpp>

#include <qubic-api/inc/Registry/ItemRegistry.hpp>
#include <qubic-api/inc/Registry/BlockRegistry.hpp>
#include <qubic-api/inc/Registry/RegistryObject.hpp>

#include <qubic-api/inc/ActionContext/ItemActionContext.hpp> // IAC

#include <cstdio>

void OnTestItemUse(Qubic::ItemActionContext* ctx) {
    printf("[TestMod] Item used!\n");
    printf("[TestMod] ========================================\n");
    printf("[TestMod] Item used!\n");
    printf("[TestMod] Player: %p\n", ctx->player);
    printf("[TestMod] World: %p\n", ctx->world);
    printf("[TestMod] ItemStack: %p\n", ctx->item_stack);
    printf("[TestMod] JNIEnv: %p\n", ctx->env);
    printf("[TestMod] ========================================\n");
    
    ctx->success = true;
    ctx->consume_item = false;
}

void init(Qubic::ModState* state) {

}

struct TestMod final : public Qubic::BaseMod {
public:
    static constexpr const char* ID = "testmod";
    void init(Qubic::ModState* state) {
        Qubic::RegisterItem(state, "test_item",     { 64, "Test Item" });
        Qubic::RegisterItem(state, "other_item",    { 64, "Other Item", true });

        Qubic::RegisterItem(state, "ruby_item",     { 64, "Ruby"});

        Qubic::ItemDescriptor desc;
        desc.max_stack = 64;
        desc.name = "Callback Item";
        desc.on_use = OnTestItemUse;
        
        Qubic::RegisterItem(state, "callback_item", desc);
    }

    void on_tick(Qubic::ModState* state) {

    }
}; 

MOD_EXPORT Qubic::ModState* mod_load(Qubic::ModState* state) {
    printf("mod loaded!\n");
    TestMod* Mod = new TestMod();

    state->mod_id = TestMod::ID;
    state->data_ptr = Mod;

    printf("'%s' loaded!\n", TestMod::ID);

    Mod->init(state);

    return state;
}