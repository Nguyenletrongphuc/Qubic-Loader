#ifndef REGISTRYOBJECT_HPP
#define REGISTRYOBJECT_HPP

#include <string>

struct RegistryObject {
public:
    std::string id;
    virtual ~RegistryObject() = default;
    const std::string& get_id() const { return this->id; }
};

#endif