#pragma once
#include "Layer_group.h"
#include "Layer.h"
#include <Event/Event_system.h>
#include <vector>
#include <Core/Log.h>
namespace Cecilion {

    /**
     * This class holds all application layer groups.
     * TODO It's not possible to remove layers.
     */
    class Layer_stack {
        using ID = int;
    public:
        Layer_stack() {}

        ID append_group() {
            this->m_groups.push_back(std::make_shared<Layer_group>());
            this->m_group_mappings[this->id_counter] = this->m_groups.back();
            this->active_layer = id_counter;
            return id_counter++;
        }

        /**
         * Update all layer groups from front to back.
         */
        void on_update() {
            for (auto& group : this->m_groups) {
                group->on_update();
            }
        }

        /**
         * Append a new layer to the current active layer group.
         * @param layer - The layer you want to put on top.
         */
        template<typename... Events>
        void append_layer(std::unique_ptr<Layer<Events...>> layer) {
            this->m_group_mappings[this->active_layer]->append_layer(std::move(layer));
            (this->add_subscription<Events>(),...);
        }

        void set_active_group(ID group_ID) {
            this->active_layer = group_ID;
        }

        void hide_group(ID group_ID) {
            this->m_group_mappings[group_ID]->hide();
        }

        void show_group(ID group_ID) {
            this->m_group_mappings[group_ID]->show();
        }

        /**
         * Force a group to be set on top of all other groups.
         * @param group_ID
         */
        void move_on_top(ID group_ID) {
            auto iterator = std::find(this->m_groups.begin(), this->m_groups.end(), this->m_group_mappings[group_ID]);
            if (iterator != this->m_groups.end()) {
                this->m_groups.push_back(*iterator);
                this->m_groups.erase(iterator);
            }
        }

        /**
         * Not tested, but it should work. Lemme know if it doesn't :)
         * @param group_ID
         */
        void delete_group(ID group_ID) {
            auto iterator = std::find(this->m_groups.begin(), this->m_groups.end(), this->m_group_mappings[group_ID]);
            if (iterator != this->m_groups.end()) {
                this->m_groups.erase(iterator);
            }
        }
    private:

        /**
         * Use this whenever a layer requests a new event subscription.
         * @tparam Event
         */
        template<typename Event>
        void add_subscription() {
            /// Check if the subscription does not exist yet, then add a callback subscription to the event.
            if (std::find(this->subscriptions.begin(), this->subscriptions.end(), typeid(Event)) == this->subscriptions.end()) {
                // TODO add subscription
                /*
                Event_system::subs<Event>([this](auto ID){
                    auto iterator = this->m_groups.rbegin();
                    while(iterator != this->m_groups.rend()) {
                        if ((*iterator)->template try_forward<Event>(ID)) {
                            break;
                        }
                        iterator ++;
                    }
                });*/
            }
        }

        /**
         * This function is empty.
         */
        void remove_subscription() {
            /// TODO Big todo.
        }

        std::vector<std::shared_ptr<Layer_group>> m_groups;
        std::unordered_map<ID, std::shared_ptr<Layer_group>> m_group_mappings;
        std::vector<std::type_index> subscriptions;
        ID id_counter = 0;
        ID active_layer = 0;
    };
}
