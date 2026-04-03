#pragma once
#include <ECS/ECS.h>
#include <Utils/Serializable.h>
#include <Utils/Type.h>
#include "ECS/I_Component.h"

namespace Cecilion {
    class Transform: public Cecilion::I_Component, public Cecilion::Serializable {
        friend class ECS;
    public:
        explicit Transform(const Cecilion::Entity_ref& ref): Cecilion::I_Component(ref) {
            if (this->m_parent != nullptr) {
                //this->m_parent.operator->().m_children.push_back(this->get_component<Transform>());
            }
        }
        Transform(Transform&& other) noexcept : Cecilion::I_Component(std::move(static_cast<I_Component&&>(other))), x(other.x), y(other.y), z(other.z), m_parent(other.m_parent) {
            other.m_parent.operator=(nullptr);
        }

        Transform& operator=(Transform&& other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            //this->m_children = std::move(other.m_children);
            this->m_parent = std::move(other.m_parent);
            return *this;
        }

        ~Transform() override = default;

        void setParent(Component_ref<Transform>& new_parent);

        YAML::Node serialize() override {
            YAML::Node n;
            n[Cecilion::Serializable::s_type_declaration] = Cecilion::type(*this);
            n["x"] = this->x;
            n["y"] = this->y;
            n["z"] = this->z;
            return n;
        }

        Serializable &operator=(const YAML::Node &serializedNode) override {
            this->x = serializedNode["x"].as<float>();
            this->y = serializedNode["y"].as<float>();
            this->z = serializedNode["z"].as<float>();
            if (serializedNode["m_parent"]) {

            }
            return *this;
        }

    private:

        float x = 0,y = 0,z = 0;
        Component_ref<Transform> m_parent;
        Component_ref<Transform> m_first_child;
        Component_ref<Transform> m_next_sibling;
        Component_ref<Transform> m_prev_sibling;
    };
}

