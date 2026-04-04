#include "ECS.h"
#include "Config.h"
#include <iostream>
#include <ranges>
#include <Utils/Type.h>

namespace Cecilion {

    ECS::~ECS() {
        auto storageCopy = this->m_component_storage;
        this->m_component_storage.clear();
        for (auto &val: storageCopy | std::views::values) {
            delete val;
        }
    }

    void ECS::delete_entity(const Entity_ref &reference) {
        if (reference.id() == ECS_NULL_ENTITY) return;

        // TODO change to entity storage has id for entity metadata
        // Check this.has_component for the ID
        if (!this->has_component<Entity_metadata>(reference.id())) {
            // TODO Error. Already deleted.
            throw std::exception();
        }

        //this->m_entities[reference].m_entity_ID = ECS_NULL_ENTITY;

        for (auto &val: this->m_component_storage | std::views::values) {
            auto storage = val;
            if (storage->has_ID(reference.id())) {
                auto entity = storage->unsafe_get(reference.id());
                storage->try_delete(reference.id());
                /*
                if(entity->m_refs == 0) {
                    storage->try_delete(reference.id());
                } else {
                    // There are still references to this object. It cannot be deleted.
                    // This function call will mark the object as deleted.
                    storage->unsafe_get(reference.id())->Destroy();
                }*/
            }
        }

        /*
        if (this->m_entities[reference].m_refs == 0) {

            for (auto & it : this->m_component_storage) {
                auto storage = (Entity_storage<I_Component>*)(it.second);
                if (storage->has_ID(reference)) {
                    if(storage->unsafe_get(reference)->m_refs > 0) {
                        // TODO Error. There can't exist any components with references when
                        // The entity does not.
                        throw std::exception();
                    }
                }
            }

            // Completely delete the entity once all references have been deleted.
            this->m_entities.free(reference);
        }*/
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


        if (!this->m_component_storage.contains(typeid(Entity_metadata))) {
            this->m_component_storage[typeid(Entity_metadata)] = new Entity_storage<Entity_metadata>();
        }

        // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
        auto ref =  Entity_ref();


        // TODO exception
        const auto id = dynamic_cast<Entity_storage<Entity_metadata> *>(this->m_component_storage[typeid(Entity_metadata)])->emplace(ref);

        // The ID referenced by metadata is initially empty. Set the proper ID returned from emplace funtion.
        this->get_component<Entity_metadata>(id).m_entity.m_source = id;
        ref.m_source = id;
        ref.p_ecs = this;
        return ref;
    }

    Entity_ref ECS::create_entity(const YAML::Node &yaml) {
        // TODO
        return Entity_ref(this, 0);
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
                // TODO Better error
                throw std::runtime_error("Serialization registry does not contain component type");
            }
            return reg[type_key](this,yaml,entity_reference);
        } else {
            // TODO Better error
            throw std::runtime_error("could not find type declaration in yaml while deserializing component");
        }
    }

    YAML::Node ECS::serialize(const Entity_ref &entity) const {
        YAML::Node node;
        node[Cecilion::Serializable::s_id_declaration] = entity.id();
        for (const auto &[fst, snd] : this->m_component_storage) {
            auto storage = snd;
            if (storage->has_ID(entity.id())) {
                if (dynamic_cast<Serializable*>(storage->unsafe_get(entity.id()))) {
                    // Wohoo the object type is serializable!
                    node[Serializable::s_component_list_declaration].push_back((dynamic_cast<Serializable*>(storage->unsafe_get(entity.id())))->serialize());
                }
            } else {
                std::cout << "Component type " << fst.name() << " is not serializable" << std::endl;
            }
        }
        return node;
    }

    bool Entity_metadata::operator==(const Entity_metadata &other) const {
        return this->m_entity_ID == other.m_entity_ID;
    }

    bool Entity_metadata::operator!=(const Entity_metadata &other) const {
        return this->m_entity_ID != other.m_entity_ID;
    }


    I_Component_ref::I_Component_ref(const Entity_ref &entity): m_entity(std::forward<const Entity_ref>(entity)){
        if (entity == nullptr) {
            // TODO Error
            throw std::exception();
        }

    }
}
