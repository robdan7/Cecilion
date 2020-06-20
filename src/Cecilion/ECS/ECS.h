#pragma once

#include <memory>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <stdint.h>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include "Component_storage.h"
#include "Filter.h"
#include "Config.h"
namespace Cecilion {

    class ECS {
    public:
        static const Entity MAX_VALUE = std::numeric_limits<Entity>::max()-1;
        static const Entity NULL_ENTITY = std::numeric_limits<Entity>::max();    /// Reserved null pointer.

        /**
         * Create a new component and return the unique component ID. This function
         * can be called by some constructor to automate the Entity creation process.
         *
         * An entity is just a 16 bit integer, i.e. the system support up to 65536-1 entities.
         * @tparam Component
         * @tparam Args
         * @param entity
         * @param component
         */
        static Entity create_entity();

        static void delete_entity(Entity entity);


        template<typename Component, typename... Args>
        static void add_component(Entity entity, Args&&... args) {
            if (!container_mappings.count(typeid(Component))) {
                container_mappings[typeid(Component)] = containers.size();
                containers.push_back(std::shared_ptr<Cecilion::Sparse_set<Entity>>(new Cecilion::Component_storage<Entity, Component>()));
                (static_cast<Component_storage<Entity, Component>*>(&(*containers.back())))->put(entity, std::forward<Args>(args)...);
            } else {
                (static_cast<Component_storage<Entity, Component>*>(&(*containers.at(container_mappings[typeid(Component)]))))->put(entity, std::forward<Args>(args)...);
            }
        }


        /**
         * Assign empty component container if the Component type does not exist.
         * This is usefull if a filter is created before any component has been added.
         * The filter will then do nothing until components of all groups exist.
         * @tparam First
         */
        template<typename First>
        static Component_storage<uint16_t, First>* assign_new_component() {
            if (!container_mappings.count(typeid(First))) {
                container_mappings[typeid(First)] = containers.size();
                containers.push_back(std::shared_ptr<Cecilion::Sparse_set<uint16_t>>(new Cecilion::Component_storage<uint16_t, First>()));
            }
            return static_cast<Component_storage<Entity, First>*>(containers.at(container_mappings[typeid(First)]).get());
        }

        template<typename... Component>
        static Cecilion::Filter<Component...> get_filter() {
            return {assign_new_component<Component>()...};
        }


        template<typename Component>
        static Component& get_component(Entity entity) {
            return (static_cast<Component_storage<Entity, Component>*>(&(*containers.at(container_mappings[typeid(Component)]))))->get(entity);
        }

    private:
        static std::unordered_map<std::type_index, int> container_mappings;
        static std::vector<std::shared_ptr<Sparse_set<Entity>>> containers;
        static std::vector<Entity> entities;
        static Entity last_deleted;
    };

}