#include "Entity.h"
#include "ECS.h"

namespace Cecilion {



    Entity_ref::~Entity_ref()  {
        if (this->p_source != nullptr && this->p_source->operator!=(nullptr)) {
            this->p_source->m_refs --;
            if(this->p_source->m_refs == 0) {
                // Delete entity
                this->p_ecs->delete_entity(this->p_source->m_entity_ID);
            }
        }
    }

    Entity_ref::Entity_ref(const Entity_ref &other) noexcept: p_ecs(other.p_ecs), p_source(other.p_source) {
        this->p_source->m_refs++;
    }

    Entity_ref::Entity_ref(Entity_ref && other) noexcept: p_source(other.p_source), p_ecs(other.p_ecs) {
        other.p_source = nullptr;
        other.p_ecs = nullptr;
    }

    Entity_ref &Entity_ref::operator=(Entity_ref &&obj) noexcept  {
        this->p_source = obj.p_source;
        this->p_ecs = obj.p_ecs;
        obj.p_source = nullptr;
        obj.p_ecs = nullptr;
        return *this;
    }

    Entity_ref &Entity_ref::operator=(const Entity_ref &other) {
        if (this == &other) return *this;
        this->p_source = other.p_source;
        this->p_ecs = other.p_ecs;
        std::ref(this->p_source) ++;

        return *this;
    }

    Entity_ref::Entity_ref(ECS *ecs, Entity_source* source): p_ecs(ecs), p_source(source) {
        if (this->p_source == nullptr || p_ecs == nullptr) {
            // Error
        } else {
            this->p_source->m_refs ++;
        }
    }

    Entity_source &Entity_ref::operator->() {
        return *this->p_source;
    }

    const Entity_ref::Entity_ID& Entity_ref::id() const {
        return this->p_source->m_entity_ID;
    }

    bool Entity_ref::operator==(nullptr_t const &_) const {
        return this->p_source == nullptr || this->p_source->operator==(nullptr);
    }

    bool Entity_ref::operator!=(nullptr_t const &_) const {
        return  this->p_source != nullptr && this->p_source->operator!=(nullptr);
    }

    bool Entity_ref::operator==(const Entity_ref &rhs) const {
        if (this->p_source != nullptr && rhs.p_source != nullptr) {
            return this->p_source->operator==(*rhs.p_source);
        }
        // The expression is never false if one or both entity sources are null.
        return false;
    }

    void Entity_ref::Destroy() {
        this->p_ecs->delete_entity(this->id());
    }

    I_Component_ref Entity_ref::add_component(const YAML::Node &node) {
        return this->p_ecs->add_component(node,*this);
    }


}