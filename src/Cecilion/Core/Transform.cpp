#include "Transform.h"

namespace Cecilion {
    REGISTER_SERIALIZABLE_COMPONENT(Transform);

    void Transform::setParent(Component_ref<Transform>& new_parent) {
        if (this->m_parent.operator==(new_parent)) return;

        // cheeky way to get component to this object.
        const Cecilion::Component_ref<Transform> self_ref = this->get_component<Transform>();

        // This removes this object from the old parent.
        /*
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
        */
    }
};