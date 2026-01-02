#ifndef ITEMREGISTRY_HPP
#define ITEMREGISTRY_HPP

#include <qubic-api/inc/Qubic.hpp>
#include <qubic-api/inc/Registry/RegistryObject.hpp>

namespace Qubic {
    struct ItemActionContext;
    typedef void (*ItemUseCallback)(ItemActionContext* ctx);

    struct ItemDescriptor final {
    public:
        int max_stack = 64;
        std::string name = "";
        bool fire_resistant = false;
        int durability = 0;

        ItemUseCallback on_use = nullptr;
    };

    struct Item final : public RegistryObject {
    private:
        Qubic::ItemDescriptor desc;
        std::string id;
    public:
        std::string GetID() { return this->id; }
        std::string SetID(std::string new_id) { return this->id = new_id; }
        
        void SetItemDescriptor(Qubic::ItemDescriptor new_desc); 
    };

    [[nodiscard]] Qubic::Item* RegisterItem(Qubic::ModState* state, const std::string& itemid, const Qubic::ItemDescriptor& desc);
}

#endif