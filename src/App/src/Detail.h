#pragma once

#include <map>
#include <string>
#include <utility>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "I_Serializable.h"
#include <ECS/Entity_ref.h>

namespace Cecilion {
    class I_Serializable;
    namespace Serializable2::Detail {
        //typedef I_Serializable (*CreateSerializableFunc)(const YAML::Node&);
        typedef Cecilion::I_Serializable (*DeserializeComponentFunc)(const YAML::Node&);

        typedef std::map<std::string, DeserializeComponentFunc> SerializableRegistry;

        inline SerializableRegistry &getSerializableRegistry() {
            static SerializableRegistry reg;
            return reg;
        }

        /*
        template<class T, std::enable_if<std::is_base_of<I_Serializable, T>::value>::type* = nullptr>
        I_Serializable *createSerializable(const YAML::Node& serializedNode) {
            // This is the function that creates an object from serialized data.

            return new T(serializedNode);
        }*/

        template<class T, std::enable_if<std::is_base_of<I_Serializable, T>::value>::type* = nullptr>
        I_Serializable deserialize_component(const YAML::Node& node,const Cecilion::Entity_ref& entity, std::shared_ptr<Cecilion::ECS>& ecs) {
            return entity.add_component<T>();
        }

        template<class T, std::enable_if<std::is_base_of<I_Serializable, T>::value>::type* = nullptr>
        struct RegistryEntry {
        public:
            static RegistryEntry<T> &Instance(const std::string &name) {
                static RegistryEntry<T> inst(name);
                return inst;
            }

        private:
            explicit RegistryEntry(const std::string &name) {
                SerializableRegistry &reg = getSerializableRegistry();
                DeserializeComponentFunc func = deserialize_component<T>;

                std::pair<SerializableRegistry::iterator, bool> ret = reg.insert(
                        SerializableRegistry::value_type(name, func));

                if (!ret.second) {
                    std::cout << "Serializable already registered to this name!!";
                }
            }

            RegistryEntry(const RegistryEntry<T> &) = delete;

            RegistryEntry &operator=(const RegistryEntry<T> &) = delete;
        };
    }


}
