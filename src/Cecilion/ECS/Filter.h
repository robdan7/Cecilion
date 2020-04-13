#pragma once

#include "Component_storage.h"
#include <Core/Log.h>
#include "Config.h"
namespace Cecilion {
    template<typename... Component>
    class Filter {
    public:
        Filter(Cecilion::Component_storage<Entity,Component>*... com) {
            this->containers = {com...};
        }

        auto get_smallest_sparse () {
            /// Expand all containers into an array and find the one with least pages.
            return std::min({
                                    static_cast<const Cecilion::Sparse_set<Entity>*>(
                                            std::get<Cecilion::Component_storage<Entity,Component>*>(this->containers)
                                    )...
                            }
                    ,[] (const auto* left, const auto* right) {
                        return left->sparse_size() < right->sparse_size();
                    });
        }

        auto get_smallest_packed() {
            return std::min({
                                    static_cast<const Cecilion::Sparse_set<Entity> *>(
                                            std::get<Cecilion::Component_storage<Entity, Component> *>(this->containers)
                                    )...
                            }, [](const auto *left, const auto *right) {
                return left->packed_size() < right->packed_size();
            });
        }

        template<typename Function>
        void for_each_sparse(Function func) {
            auto smallest_container = this->get_smallest_sparse();
            int page_size = smallest_container->sparse_size();

            for (int page = 0; page < page_size; page++) {
                Entity ID = 0;
                for (int offset = 0; offset < smallest_container->index_per_page; offset++) {
                    ID = smallest_container->ID(page, offset);
                    if (auto all = (std::get<Cecilion::Component_storage<Entity, Component> *>(containers)->has_ID(
                                ID) &&...); all) {
                        if constexpr (std::is_invocable_v<Function, decltype(std::get<Cecilion::Component_storage<Entity, Component> *>(
                                containers)->get(ID))...>) {
//                        my_test_function(std::get<Cecilion::Component_storage<Entity,Comps>*>(containers)->get(ID)...);
                            func(std::get<Cecilion::Component_storage<Entity, Component> *>(containers)->get(ID)...);
                        } else {
                            CORE_ASSERT(false, "Error: ECS: Component callback function does not contain the right types");
                            LOG_CRITICAL("A ECS function is NOT invocable");
                        }
                    }
                }
            }
        }

        /**
         * Compact iteration function that iterates only over all valid IDs.
         * TODO Performance test.
         * @tparam Function
         * @param func
         */
        template<typename Function>
        void for_each_compact(Function func) {
            auto smallest_container = this->get_smallest_packed();
            smallest_container->for_each_packed([this, func](auto ID){
                if (auto all = (std::get<Cecilion::Component_storage<Entity, Component> *>(containers)->has_ID(
                            ID) &&...); all) {
                    if constexpr (std::is_invocable_v<Function, decltype(std::get<Cecilion::Component_storage<Entity, Component> *>(containers)->get(ID))...>) {
//                        my_test_function(std::get<Cecilion::Component_storage<Entity,Comps>*>(containers)->get(ID)...);
                        func(std::get<Cecilion::Component_storage<Entity, Component> *>(containers)->get(ID)...);
                    } else {
                        CORE_ASSERT(false, "Error: ECS: Component callback function does not contain the right types");
                        LOG_CRITICAL("A ECS function is NOT invocable");
                    }
                }


            });
        }

        std::tuple<Cecilion::Component_storage<Entity, Component>*...> containers;
    };
}