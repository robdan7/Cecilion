#pragma once
//#include "ECS.h"
#include "Entity_metadata.h"
namespace Cecilion {
/*
    template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> *, typename ... Args>
    Component_ref<C> ECS::add_component(const uint16_t &entity, const Entity_ref &entity_reference, Args... args) {
        //ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
        // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
        if (!this->m_component_storage.contains(typeid(C))) {
            this->m_component_storage[typeid(C)] = new Entity_storage<C>();
        }

        if (this->m_component_storage[typeid(C)]->has_ID(entity)) {
            // TODO Error
            throw std::runtime_error("Entity system does not contain any entity with this ID");
        }

        // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
        Event_system::post<Events::Component_event<C>>(true);
        static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->emplace_entry(entity,
                                                                                              entity_reference,
                                                                                              std::forward<Args>(
                                                                                                      args)...);

        static_cast<Entity_storage<Entity_metadata> *>(this->m_component_storage[typeid(Entity_metadata)])->emplace(entity_reference);
        return Component_ref<C>(entity_reference);
    }*/
/*
    template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>> *, std::enable_if_t<std::is_base_of_v<Cecilion::I_Component, T>> *>
    void ECS::register_serializable_component_type(const std::string &name) {
        DeserializeComponentFunc func = ECS::add_component<T>;
        getSerializableComponentRegistry().insert(SerializableComponentRegistry::value_type(name, func));

        DeserializeComponentFunc ref_func = I_Component_ref::deserialize_helper<T>;
        getSerializableComponentRegistry().insert(SerializableComponentRegistry::value_type (name,ref_func));
    }
*/
    /*
    template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>> *E0, std::enable_if_t<std::
        is_base_of_v<Cecilion::I_Component, T>> *E1>
    ECS::Serializable_component_entry<T> & ECS::Serializable_component_entry<T, E0, E1>::Instance(
    const std::string &name)  {
        static Serializable_component_entry<T> inst(name);
        return inst;
    }

*/
/*
    template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> *>
    Component_ref<C> Entity_ref::add_component() {
        return this->p_ecs->add_component<C>(this->id(),*this);
    }
*/
    /*
    template<class C>
    Component_ref<C> I_Component::add_component() {
        return this->m_entity.add_component<C>();
    }*/
}