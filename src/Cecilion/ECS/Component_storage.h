#pragma once

#include <Utils/Sparse_set.h>
namespace Cecilion {

    template<typename Entity, typename Component>
    class Component_storage : public Sparse_set<Entity>{
    public:
        Component_storage() : components(std::vector<Component>()) , Sparse_set<Entity>() {
        }

        /**
         * Add a new component to the storage.
         * @tparam Args
         * @param entity - The unique entity ID.
         * @param component
         * @param args - Arguments in the same order as the component constructor would need.
         */
        template <typename... Args>
        void put(Entity entity, Args&&... args) {
            Sparse_set<Entity>::put(entity);


            /// https://en.cppreference.com/w/cpp/types/is_aggregate
            /// The is_aggregate function wont work unless it is checked at compile time,
            /// hence the constexpr statement.
            if constexpr (std::is_aggregate_v<Component>) {
                this->components.push_back(Component{std::forward<Args>(args)...});
            } else {
                this->components.emplace_back(std::forward<Args>(args)...);
            }
        }

        void remove(Entity entity) {
            this->components.at(this->index(entity)) = this->components.back();
            this->components.pop_back();
            this->remove(entity);
        }

        Component& get(Entity ID) {
            return this->components.at(Sparse_set<Entity>::index(ID));
        }

        auto get_sparse_list() {
            return &this->sparse_list;
        }

    private:
        std::vector<Component> components;
    };
}