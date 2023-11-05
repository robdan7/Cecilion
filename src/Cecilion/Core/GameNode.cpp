#include "GameNode.h"
#include <iostream>
#include <source_location>

namespace Cecilion {
    auto test = std::source_location::current();

    GameNode::GameNode(const Cecilion::Entity_ref &entity, const Component_ref<GameNode>& parent): I_Component(std::forward<const Entity_ref>(entity)), m_parent(parent) {
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

    GameNode::GameNode(const Entity_ref &entity) : I_Component(entity), m_parent() {

    }
}

