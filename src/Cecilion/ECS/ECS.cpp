#include "ECS.h"
#include "Config.h"

#include <Core/Log.h>
#include <iostream>
namespace Cecilion {
    ECS::~ECS() {
        auto storageCopy = this->m_component_storage;
        this->m_component_storage.clear();
        for (auto & it : storageCopy) {
            delete it.second;
        }
    }

    void ECS::delete_entity(ECS::Entity_ID ID) {
        if (ID == ECS_NULL_ENTITY) return;
        if (!this->m_entities.has_entry(ID)) {
            // TODO Error. Already deleted.
            throw std::exception();
        }

        this->m_entities[ID].m_entity_ID = ECS_NULL_ENTITY;

        for (auto & it : this->m_component_storage) {
            auto storage = it.second;
            if (storage->has_ID(ID)) {
                auto entity = storage->unsafe_get(ID);
                if(entity->m_refs == 0) {
                    storage->try_delete(ID);
                } else {
                    // There are still references to this object. It cannot be deleted.
                    // This function call will mark the object as deleted.
                    storage->unsafe_get(ID)->Destroy();
                }
            }
        }

        if (this->m_entities[ID].m_refs == 0) {

            for (auto & it : this->m_component_storage) {
                auto storage = (Entity_storage<I_Component>*)(it.second);
                if (storage->has_ID(ID)) {
                    if(storage->unsafe_get(ID)->m_refs > 0) {
                        // TODO Error. There can't exist any components with references when
                        // The entity does not.
                        throw std::exception();
                    }
                }
            }

            // Completely delete the entity once all references have been deleted.
            this->m_entities.free(ID);
        }
    }



//	void ECS::set_as_active_context() {
//		this = (ECS*)this;
//	}
/*
    void ECS::clear_storage() {

        for (auto& ID : this->m_delete_list) {
            if (std::find(this->m_freelist.begin(), this->m_freelist.end(), ID) != this->m_freelist.end()) {
                // ORVOX_ERROR("Tried to delete an entity that does not exist"); TODO Error
            }
            for (auto it = this->m_component_storage.begin(); it != this->m_component_storage.end(); ++it) {
                it->second->try_delete(ID);
            }
            this->m_freelist.push_back(ID);
        }
        this->m_delete_list.clear();
    }*/


    Entity_ref ECS::create_entity() {

        // This may throw error.
        auto id = this->m_entities.alloc();

        this->m_entities.at(id).m_entity_ID = id;

        return Entity_ref(this, &this->m_entities[id]);
    }

    Entity_ref ECS::create_entity(const YAML::Node &yaml) {
        return Entity_ref(this, nullptr);
    }

    I_Component_ref
    ECS::add_component(const YAML::Node &yaml, const Entity_ref &entity_reference) {
        if (!yaml.IsDefined() || yaml.IsNull() || !yaml.IsMap()) {
            // TODO Better error
            throw std::runtime_error("Invalid Yaml type when parsing ECS component");
        }

        if (yaml[Cecilion::Serializable::s_type_declaration]) {
            // Parse yaml.

            // Fetch registry of functions that can create components.
            std::string type_key = yaml[Cecilion::Serializable::s_type_declaration].as<std::string>();
            auto reg = ECS::getSerializableComponentRegistry();
            if (!reg.contains(type_key)) {
                // TODO BEtter error
                throw std::runtime_error("Serialization registry does not contain component type");
            }
            return reg[type_key](this,yaml,entity_reference);
        } else {
            // TODO Better error
            throw std::runtime_error("could not find type declaration in yaml while deserializing component");
        }
    }

    bool Entity_source::operator==(const Entity_source &other) const {
        return this->m_refs == other.m_refs && this->m_entity_ID == other.m_entity_ID;
    }

    bool Entity_source::operator!=(const Entity_source &other) const {
        return this->m_refs != other.m_refs || this->m_entity_ID != other.m_entity_ID;
    }

    bool I_Component::operator==(const std::nullptr_t& _) {
        return this->m_entity.operator==(nullptr);
    }

    void I_Component::Destroy() {
        this->m_entity = nullptr;
    }

    I_Component::I_Component(I_Component &&other) noexcept : m_entity(std::move(other.m_entity)) {
        other.m_entity = nullptr;
    }

    I_Component &I_Component::operator=(I_Component &&other) noexcept {
        this->m_entity.operator=(std::move(other.m_entity));
        return *this;
    }

    Entity_ref &I_Component::entity()  {
        return this->m_entity;
    }

    I_Component::I_Component(const Entity_ref &ref): m_entity(std::forward<const Entity_ref>(ref)){

    }

    I_Component_ref::I_Component_ref(const Entity_ref &entity): m_entity(std::forward<const Entity_ref>(entity)){
        if (entity == nullptr) {
            // TODO Error
            throw std::exception();
        }

    }
}