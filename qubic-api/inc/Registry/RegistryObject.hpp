#ifndef REGISTRYOBJECT_HPP
#define REGISTRYOBJECT_HPP

#define COMPILE_TIME_WARNING(msg) \
    _Pragma(#msg)

#define STRINGIFY(x) #x
#define WARNING(msg) _Pragma(STRINGIFY(message("WARNING: " msg)))

#include <string>

struct RegistryObject {
public:
    std::string id;
    virtual ~RegistryObject() = default;

    const std::string& get_id() const { return id; }
};

#endif