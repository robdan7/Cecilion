#pragma once
#include <tuple>
#include <Utils/allocator/Sparse_table.h>
#include "Config.h"
#include <algorithm>
#include <Event/Event_actor.h>
#include <Event/System_events.h>

namespace Cecilion {
	class ECS;

	/**
	 * These are the "system" part in entity component systems and are responsible for, well, iterating through
	 * entities that all contain certain components.
	 * @tparam Components - One or more components of any type. Not assigning a component leads to unspecified behaviour.
	 */
	template<class... Components>
	class Component_iterator : public Event_actor<Events::Component_event<Components>...> {
		friend ECS;
	public:
		/**
		 * Use this function to iterate through all entities that contain the specified components.
		 * @param f
		 */
		void for_each(std::function<void(ECS_ENTITY_SIZE&&, Components&...)> f) {
			Event_actor<Events::Component_event<Components>...>::template check_inbox<Events::Component_event<Components>...>();
			if constexpr (std::tuple_size<decltype(this->p_storage)>::value == 1) {
				auto table = std::get<0>(this->p_storage);
				auto& packed = table->get_packed_set();
				for (std::size_t i = 0; i < packed.size(); ++i) {
					f(table->get_ID(i), packed[i]);
				}
			}
			else {
				if (!this->m_valid_cache) {
					this->build_cache();
				}

				for (auto& entity : this->m_component_cache) {
					f((uint16_t)std::get<0>(entity), std::get<Components&>(entity)...);
				}
			}
		}

        uint32_t get_iteration_size() {
            Event_actor<Events::Component_event<Components>...>::template check_inbox<Events::Component_event<Components>...>();
            if constexpr (std::tuple_size<decltype(this->p_storage)>::value == 1) {
                auto table = std::get<0>(this->p_storage);
                auto& packed = table->get_packed_set();
                return packed.size();
            }
            else {
                if (!this->m_valid_cache) {
                    this->build_cache();
                }

                return this->m_component_cache.size();
            }
        }

		/**
		 * Retrieve the smallest guaranteed size of all filtered entities.
		 * The size might be smaller, but will never be more than returned by this function.
		 * <br>
		 * Keep in mind that the returned value is subject of change.
		 * @return
		 */
		std::size_t guaranteed_size() {
			return this->get_smallest_sparse()->packed_size();
		}

	private:

        /**
         * Build cache for multiple component types. This is faster than collecting components all the time.
         */
		void build_cache() {
			this->m_component_cache.clear();
			if constexpr (std::tuple_size<decltype(this->p_storage)>::value == 1) {
				this->m_valid_cache = true;
				return;
			}
			else {
				auto smallest = this->get_smallest_sparse();
				uint32_t max_iterations = smallest->n_sparse_pages();
				ECS_ENTITY_SIZE max_entities = smallest->packed_size();
				this->m_component_cache.reserve(max_entities);
				for (uint32_t i = 0; i < (uint32_t)1 << (8 * ECS_PAGE_BYTES) && max_iterations > 0; ++i) {
					auto page_start = i * ((uint32_t)1 << (8 * sizeof(ECS_OFFSET_TYPE)));
					if (auto all_pages = (std::get<Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Components>*>(this->p_storage)->has_table(i)&&...); all_pages) {
						max_iterations--;

						/// Run this loop for all k without calculating the max value of k.
						ECS_OFFSET_TYPE k = 0;
						do {
							if (auto all_IDs = (std::get<Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Components>*>(this->p_storage)->has_ID(page_start + k)&&...); all_IDs) {
								this->m_component_cache.emplace_back(page_start + k, (*std::get<Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Components>*>(this->p_storage))[page_start + k]...);
							}
							k++;
							max_iterations--;
						} while (k != 0 && max_entities);
					}
				}
				this->m_valid_cache = true;
			}
		}

		/**
		 *
		 * @return The sparse table with the fewest sparse pages. Less pages -> fewer iterations.
		 */
		auto get_smallest_sparse() {
			return (std::min)({ ((I_sparse_table*)(std::get<Sparse_table<ECS_ENTITY_SIZE,ECS_PAGE_BYTES,ECS_OFFSET_TYPE,Components>*>(this->p_storage)))... }, [](const auto* left, const auto* right) {
				return left->n_sparse_pages() < left->n_sparse_pages();
				});
		}

		Component_iterator(Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Components>*... containers) : Event_actor<Events::Component_event<Components>...>(), p_storage(containers...) {
			(this->template set_callback<Events::Component_event<Components>>([this](const auto& event) {
				this->m_valid_cache = false;
				}), ...);
		}
		std::tuple<Sparse_table < ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Components>*...> p_storage;
		bool m_valid_cache = false;
		std::vector<std::tuple<ECS_ENTITY_SIZE, Components&...>> m_component_cache;
	};
}
