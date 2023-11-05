#pragma once
#include <yaml-cpp/yaml.h>

namespace Cecilion {
    class Serializable {
    public:

        virtual YAML::Node serialize() = 0;
        virtual Serializable& operator=(const YAML::Node& serializedNode) = 0;

        static const char* s_type_declaration;
    };
}