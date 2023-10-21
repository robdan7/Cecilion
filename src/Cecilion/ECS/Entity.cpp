#include "Entity.h"
#include "ECS.h"

namespace Cecilion {



    Entity_ref::~Entity_ref()  {
        if (this->p_source != nullptr) {
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



    Entity_ref Entity_ref::Instantiate() {
        if (this->operator==(nullptr)) {
            // TODO Error
        }
        return this->p_ecs->create_entity();
    }

    void Entity_ref::delete_compoent(std::type_index type) {
        this->p_ecs->pop_component(*this,type);
    }

    I_Component& Entity_ref::get_component(std::type_index type) {
        return this->p_ecs->get_component(*this,type);
    }

    ECS *Entity_ref::get_ecs() {
        return this->p_ecs;
    }

    template<typename C>
    C &Entity_ref::get_component() {
        if (!this->p_ecs->has_component<C>(this->id())) {
            // TODO Error
        }
        return this->p_ecs->get_component<C>(this->p_source->m_entity_ID);
    }

}