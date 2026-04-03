#include "GameNode.h"
#include <iostream>
#include <source_location>

namespace Cecilion {
    //REGISTER_SERIALIZABLE_COMPONENT(GameNode);

    GameNode::GameNode(const Entity_ref &entity) : I_Dependency_component<Cecilion::Transform>(entity), m_transform(this->get_component<Transform>()) {
        if (this->entity() == nullptr) {
            // TODO Better error
            throw std::runtime_error("Could not initiate game node");
        }
    }

    Cecilion::Component_ref<Transform> GameNode::transform() {
        return this->m_transform;
    }


    GameNode::GameNode(GameNode &&other): I_Dependency_component<Cecilion::Transform>(other.entity()), m_transform(other.m_transform) {
        other.m_transform = nullptr;
    }

    GameNode &GameNode::operator=(GameNode &&other) {
        this->m_transform = std::move(other.m_transform);
        I_Component::operator=(std::move(other));

        return *this;
    }
}

