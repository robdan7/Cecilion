#pragma once

#include <memory>
#include <utility>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include "Component_storage.h"
#include "Filter.h"
#include "Config.h"
#include <Utils/allocator/Sparse_table.h>
#include <Event/System_events.h>
#include "Component_iterator.h"
#include "Entity.h"
#include "Component.h"
namespace Cecilion {

    /**
     * This is the global entity component system. A lot of inspiration has been taken from
     * ENTT on GitHub and other various sources on the web. It actually works though.
     */
    class ECS {
    private:
        /**
         * The only purpose of this class is to properly send events when components are deleted.
         * We don't know what type of components to delete when deleting an entity, so the
         * event must be posted from a sparse table. Otherwise we would need one event type for all
         * components, even if not all component iterators are required to refresh their cache every time
         * an entity is deleted.
         * @tparam C
         */
        template<class C>
        class Entity_storage : public Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C> {
        public:

            void delete_entry(const size_t& ID) override {
                Event_system::post<Events::Component_event<C>>(false);
                Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C>::delete_entry(ID);
            }

            bool try_delete(const size_t& ID) override {
                if (this->has_ID(ID)) {
                    this->delete_entry(ID);
                    return true;
                }
                return false;
            }
        };

    public:
        using Entity_ID = ECS_ENTITY_SIZE;

        /*
        ECS(const ECS& other) : m_freelist(other.m_freelist), m_delete_list(other.m_delete_list), m_allocated_entities(other.m_allocated_entities){
            for (auto it = other.m_component_storage.begin(); it != other.m_component_storage.end(); ++it) {
                this->m_component_storage[it->first] = it->second->make_copy();
            }
        }*/
        /*
        ECS* operator=(const ECS& other) {
            this->m_allocated_entities = other.m_allocated_entities;
            this->m_delete_list = other.m_delete_list;
            this->m_freelist = other.m_freelist;

            for (auto it = this->m_component_storage.begin(); it != this->m_component_storage.end(); ++it) {
                delete this->m_component_storage[it->first];
                this->m_component_storage[it->first] = nullptr;
            }
            this->m_component_storage.clear();

            this->m_component_storage.clear();
            for (auto it = other.m_component_storage.begin(); it != other.m_component_storage.end(); ++it) {
                this->m_component_storage[it->first] = it->second->make_copy();
            }
            return this;
        }*/
        ECS(const ECS& other) = delete;
        ECS(ECS&& other) = delete;
        ECS() = default;
        ~ECS();
        Entity_ref create_entity();
        void delete_entity(const Entity_ID& ID);
        void clear_storage();

        /**
         * Create a component iterator
         * @tparam Components
         * @return
         */
        template<typename... Components>
        Component_iterator<Components...> create_iterator() {
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            return { this->assign_storage<Components>()... };
        }

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type* = nullptr, typename... Args>
        Component_ref<C>& add_component(const Entity_ref& entity, Args... args) {
            //ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
            }

            if (this->m_component_storage[typeid(C)]->has_ID(entity.id())) {
                // TODO Error
            }

            // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
            Event_system::post<Events::Component_event<C>>(true);
            static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->emplace_entry(entity.p_source->m_entity_ID,std::forward<const Entity_ref>(entity), std::forward<Args>(args)...);
            return Component_ref<C>(entity);
        }

        /*
        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type* = nullptr>
        Component_ref<C>& push_component(const Entity_ref& entity, const C& arg) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
            }
            // ORVOX_TRACE("Pushed {0} component for ID {1}", typeid(C).name(), ID); TODO Error
            Event_system::post<Events::Component_event<C>>(true);
            static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->push_entry(entity.p_source->m_entity_ID, arg);
            return Component_ref<C>(entity);
        }*/

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type* = nullptr>
        C& get_component(const Entity_ref& entity) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                // TODO Log message.
                // ORVOX_ERROR("Could not fetch component of type {0}", typeid(C).name());
                //throw std::invalid_argument("Entity system does not contain any entities of this type");
                // TODO Error
            }
            Entity_storage<C>* container = static_cast<Entity_storage<C>*>(this->m_component_storage[typeid(C)]);
            // ORVOX_ASSERT(container->has_ID(ID), "Tried to acces invalid component");
            if (!container->has_ID(entity.p_source->m_entity_ID)) {
                // TODO Error
            }
            return (*container)[entity.p_source->m_entity_ID];
        }

        I_Component& get_component(const Entity_ref& entity, std::type_index type) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(type) == 0) {
                // TODO Log message.
                // ORVOX_ERROR("Could not fetch component of type {0}", typeid(C).name());
                //throw std::invalid_argument("Entity system does not contain any entities of this type");
                // TODO Error
            }
            auto* container = static_cast<Entity_storage<I_Component>*>(this->m_component_storage[type]);
            // ORVOX_ASSERT(container->has_ID(ID), "Tried to acces invalid component");
            if (!container->has_ID(entity.p_source->m_entity_ID)) {
                // TODO Error
            }
            return (*container)[entity.p_source->m_entity_ID];
        }

        template<typename C>
        void pop_component(const Entity_ID& ID) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context");
            if (this->m_component_storage.count(typeid(C)) == 0) {
                // ORVOX_ERROR("Could not delete component of type {0}", typeid(C).name());
                throw std::invalid_argument("Entity system does not contain any entities of this type");
            }
            Entity_storage<C>* container = static_cast<Entity_storage<C>*>(this->m_component_storage[typeid(C)]);
            if (!container->try_delete(ID)) {
                // ORVOX_ERROR("Tried to delete {0} from entity {1}, but no component was found!", typeid(C).name(), ID);
                return;
            }
        }

        void pop_component(const Entity_ref& entity, std::type_index type) {
            if (this->m_component_storage.count(type) == 0) {
                // ORVOX_ERROR("Could not delete component of type {0}", typeid(C).name());
                throw std::invalid_argument("Entity system does not contain any entities of this type");
            }
            auto container = this->m_component_storage[type];
            if (!container->try_delete(entity.id())) {
                // ORVOX_ERROR("Tried to delete {0} from entity {1}, but no component was found!", typeid(C).name(), ID);
                return;
            }
        }

        template<class C>
        bool has_component(const Entity_ID& ID) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            if (this->m_component_storage.count(typeid(C)) == 0) {
                return false;
            }
            Entity_storage<C>* container = static_cast<Entity_storage<C>*>(this->m_component_storage[typeid(C)]);
            return container->has_ID(ID);
        }

    private:

        template<typename C>
        Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C>* assign_storage() {
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
                // ORVOX_TRACE("Assigned component storage for {0}", typeid(Component).name());
            }

            return static_cast<Entity_storage<C>*>(this->m_component_storage[typeid(C)]);
        }

    private:

        std::unordered_map<std::type_index, I_sparse_table*> m_component_storage;

        // TODO This has the possibility to fill up.
        //std::vector<Entity_ID> m_freelist;

        //std::vector<Entity_ID> m_delete_list;
        //Entity_ID m_allocated_entities = ECS_NULL_ENTITY+1;

        Same_arena_allocator<ECS_PAGE_BYTES,ECS_OFFSET_TYPE,Entity_ref::Entity_source> m_entities;
    };

}