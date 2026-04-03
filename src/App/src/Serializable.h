#pragma once
#include <string>
#include "Detail.h"
#include <yaml-cpp/yaml.h>
#include "I_Serializable.h"
namespace Cecilion {


    namespace Serializable2 {

        //void DeserializeComponent(const Cecilion::Entity_ref& entity, const YAML::Node& serializedComponent);

        Cecilion::I_Serializable DeserializeComponent(const std::string& name, const YAML::Node& node);

        //I_Serializable Deserialize(const std::string& name, const YAML::Node& serializedNode);
    }

#define SERIALIZABLE_REGISTER(TYPE) \
    namespace Cecilion::Serializable::Detail { \
        template<class T>           \
        class SerializableRegistraction;       \
    \
    template<>                      \
    class SerializableRegistraction<TYPE> {    \
        static const Serializable::Detail::RegistryEntry<TYPE>& reg; \
    };                              \
    const ::Cecilion::Serializable::Detail::RegistryEntry<TYPE>&                  \
        SerializableRegistraction<TYPE>::reg =     \
            ::Cecilion::Serializable::Detail::RegistryEntry<TYPE>::Instance(typeid(TYPE).name());\
            \
    }                               \

}