#include <qubic-api/inc/Qubic.hpp>
#include <cstdio>

struct ModHelper final : public Qubic::BaseMod {
public:
    static constexpr const char* ID = "qubicmodhelper";
    void init(Qubic::ModState* state) { printf("[Qubic] Mod helper initialized!\n"); }
}; 

MOD_EXPORT Qubic::ModState* mod_load(Qubic::ModState* state) {
    ModHelper* Mod = new ModHelper();

    state->mod_id = ModHelper::ID;
    state->data_ptr = Mod;

    Mod->init(state);

    return state;
}