#include "Serializable.h"
#include <string>
#include "Detail.h"

namespace Cecilion {

    /**
     * Frontend function that finds an internal function
     * @param name
     * @return
     */
     /*
    I_Serializable* Serializable::Deserialize(const std::string &name, const YAML::Node& serializedNode) {
        Serializable::Detail::SerializableRegistry& reg = Serializable::Detail::getSerializableRegistry();
        auto it = reg.find(name);
        if (it == reg.end()) {
            std::cout << "Could not find serializable creator" << std::endl;
            return nullptr;
        }

        Serializable::Detail::CreateSerializableFunc func = it->second;
        return func(serializedNode);
    }*/

    Cecilion::I_Serializable
    Serializable2::DeserializeComponent(const std::string &name, const YAML::Node &node) {
        Serializable2::Detail::SerializableRegistry& reg = Serializable2::Detail::getSerializableRegistry();
        auto it = reg.find(name);
        if (it == reg.end()) {
            std::cout << "Could not find serializable creator" << std::endl;
            // Throw exception!
        }
        Serializable2::Detail::DeserializeComponentFunc func = it->second;

        return func(node);
    }
}