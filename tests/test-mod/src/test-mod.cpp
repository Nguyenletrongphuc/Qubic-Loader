#include <qubic-api/inc/Qubic.hpp>

#include <qubic-api/inc/Registry/ItemRegistry.hpp>
#include <qubic-api/inc/Registry/BlockRegistry.hpp>
#include <qubic-api/inc/Registry/RegistryObject.hpp>

#include <cstdio>

struct TestMod final : public Qubic::BaseMod {
public:
    static constexpr const char* ID = "testmod";
    void init(Qubic::ModState* state) {
        printf("called init!\n");
        /* register items, max stack in minecraft is 99 (you will get a warning if you go over )*/
        Qubic::RegisterItem(state, "test-item", {128, "Test Item", ""});
        Qubic::RegisterItem(state, "other-item", {128, "Other Item", ""});
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