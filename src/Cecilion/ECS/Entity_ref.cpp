#include "Entity_ref.h"
#include "I_Component.h"
#include "ECS.h"
#include "Entity_metadata.h"

/*
    Entity_ref::~Entity_ref()  {

        if (this->m_source != nullptr && this->m_source->operator!=(nullptr)) {
            this->m_source->m_refs --;
            if(this->m_source->m_refs == 0) {
                // Delete entity
                this->p_ecs->delete_entity(this->m_source->m_entity_ID);
            }
        }

    }

    Entity_ref::Entity_ref(const Entity_ref &other) noexcept: p_ecs(other.p_ecs), m_source(other.m_source) {
        //this->m_source->m_refs++;
    }

    Entity_ref::Entity_ref(Entity_ref && other) noexcept: p_ecs(other.p_ecs), m_source(other.m_source) {
        //other.m_source = nullptr;
        other.p_ecs = nullptr;
    }

    Entity_ref &Entity_ref::operator=(Entity_ref &&obj) noexcept  {
        this->m_source = obj.m_source;
        this->p_ecs = obj.p_ecs;
        //obj.m_source = nullptr;
        obj.p_ecs = nullptr;
        return *this;
    }

    Entity_ref & Entity_ref::operator=(std::nullptr_t &&_) {
        if (this->m_source != ECS_NULL_ENTITY) {
            auto component = this->get_component<Entity_metadata>();
            if (component.operator->().m_refs == 0) {
                // TODO Exception
                throw std::runtime_error("ECS::Entity::Entity_ref: ECS_NULL_ENTITY");
            }
            component.operator->().m_refs --;
        }
        this->m_source = ECS_NULL_ENTITY;
        this->p_ecs = nullptr;
        return *this;
    }

    Entity_ref &Entity_ref::operator=(const Entity_ref &other) {
        if (this == &other) return *this;
        this->m_source = other.m_source;
        this->p_ecs = other.p_ecs;
        std::ref(this->m_source) ++;

        return *this;
    }

    Entity_ref::Entity_ref(ECS *ecs, const ECS_ENTITY_SIZE &source): p_ecs(ecs), m_source(source) {
        if (this->m_source == ECS_NULL_ENTITY || p_ecs == nullptr) {
            // TODO Error
            throw std::runtime_error("ECS::Entity::Entity_ref: ECS_NULL_ENTITY");
        } else {
            this->get_component<Entity_metadata>().operator->().m_refs++;
        }
    }

    const Entity_ref::Entity_ID& Entity_ref::id() const {
        return this->m_source;
    }

    bool Entity_ref::operator==(nullptr_t const &_) const {
        return this->m_source == ECS_NULL_ENTITY;
    }

    bool Entity_ref::operator!=(nullptr_t const &_) const {
        return  this->m_source != ECS_NULL_ENTITY;
    }

    bool Entity_ref::operator==(const Entity_ref &rhs) const {
        return this->m_source == rhs.m_source;
    }

    void Entity_ref::Destroy() const {
        this->p_ecs->delete_entity(*this);
    }

    I_Component_ref Entity_ref::add_component(const YAML::Node &node) const {
        return this->p_ecs->add_component(node,*this);
    }*/
