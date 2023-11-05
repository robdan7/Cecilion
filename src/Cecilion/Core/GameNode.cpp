#include "GameNode.h"
#include <iostream>
#include <source_location>

namespace Cecilion {
    REGISTER_SERIALIZABLE_COMPONENT(GameNode);

    GameNode::GameNode(const Cecilion::Entity_ref &entity, const Component_ref<GameNode>& parent): I_Dependency_component<Cecilion::Transform>(std::forward<const Entity_ref>(entity)), m_parent(parent), m_transform(this->get_component<Transform>()) {
        if (this->m_entity == nullptr) {
            // TODO Better error
            throw std::runtime_error("Could not initiate game node");
        }
        if (this->m_parent != nullptr) {
            //this->m_parent.operator->().m_children.pu
        }
    }

    void GameNode::setParent(Component_ref<GameNode>& new_parent) {
        if (this->m_parent.operator==(new_parent)) return;

        // cheeky way to get component to this object.
        const Cecilion::Component_ref<GameNode> self_ref = this->get_component<GameNode>();

        // This removes this object from the old parent.
        auto& child_list = this->m_parent.operator->().m_children;
        auto it = std::find(child_list.begin(), child_list.end(), self_ref);
        if (it != child_list.end()) {
            std::size_t index = it - child_list.begin();
            if (child_list.size() > 1 && index < child_list.size() - 1) {
                child_list[index] = child_list.back();
            }
            child_list.pop_back();
        }

        // Add to new parent
        this->m_parent = new_parent;
        new_parent.operator->().m_children.push_back(self_ref);
    }

    GameNode::GameNode(const Entity_ref &entity) : I_Dependency_component<Cecilion::Transform>(entity), m_parent(), m_transform(this->get_component<Transform>()) {

    }

    Cecilion::Component_ref<Transform> GameNode::transform() {
        return this->m_transform;
    }

    YAML::Node GameNode::serialize() {
        YAML::Node node;
        node[Cecilion::Serializable::s_type_declaration] = "Potato";
        return node;
    }

    Serializable &GameNode::operator=(const YAML::Node &serializedNode) {
        return *this;
    }

    GameNode::GameNode(GameNode &&other): I_Dependency_component<Cecilion::Transform>(other.m_entity), m_transform(other.m_transform), m_parent(other.m_parent), m_children(std::move(other.m_children)) {
        other.m_parent.operator=(nullptr);
        other.m_transform = nullptr;
    }

    GameNode &GameNode::operator=(GameNode &&other) {
        this->m_children = std::move(other.m_children);
        this->m_parent = std::move(other.m_parent);
        this->m_transform = std::move(other.m_transform);
        I_Component::operator=(std::move(other));

        return *this;
    }
}

