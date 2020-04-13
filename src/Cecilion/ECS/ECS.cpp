#include "ECS.h"
#include <Core/Log.h>
#include <iostream>
namespace Cecilion {
    std::vector<std::shared_ptr<Sparse_set<Entity>>> ECS::containers = std::vector<std::shared_ptr<Sparse_set<Entity>>>();
    std::unordered_map<std::type_index, int> ECS::container_mappings = std::unordered_map<std::type_index, int>();
    std::vector<Entity> ECS::entities = std::vector<Entity>();
    Entity ECS::last_deleted = ECS::NULL_ENTITY;

    /**
     * Create an entity. There can at most exist 65515 entities at the same time. Allocating
     * more entities will lead to undefined behaviour.
     * @return
     */
    Entity ECS::create_entity() {
        CORE_ASSERT(entities.size() == ECS::MAX_VALUE, "The maximum number of ECS entities has been reached! Total: [0}", ECS::MAX_VALUE);

        if (last_deleted != ECS::NULL_ENTITY) {
            /// We need to reuse an old entity ID.
            Entity ID = last_deleted;
            last_deleted = entities[last_deleted];
            entities[ID] = ID;
            return ID;
        }
        return entities.emplace_back(entities.size());
    }

    void ECS::delete_entity(Entity entity) {
        CORE_ASSERT(entity!=ECS::NULL_ENTITY, "Cannot delete null pointer entity!");
        if (entity == NULL_ENTITY) {
            throw "ECS entity is equal to null pointer!";
        }

        for (std::shared_ptr<Sparse_set<Entity>> container : containers) {
            if (container->has_ID(entity)) {
                container->remove(entity);
            }
        }
        entities.at(entity) = last_deleted;
        last_deleted = entity;
    }

}