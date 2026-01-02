#include <qubic-api/inc/Qubic.hpp>

#include <qubic-api/inc/Registry/ItemRegistry.hpp>
#include <qubic-api/inc/Registry/BlockRegistry.hpp>
#include <qubic-api/inc/Registry/RegistryObject.hpp>

#include <cstdio>

struct TestMod final : public Qubic::BaseMod {
public:
    static constexpr const char* ID = "testmod";
    void init(Qubic::ModState* state) {
        Qubic::RegisterItem(state, "test_item",     { 64, "Test Item" });
        Qubic::RegisterItem(state, "other_item",    { 64, "Other Item", true });

        Qubic::RegisterItem(state, "ruby_item",     { 64, "Ruby"});
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