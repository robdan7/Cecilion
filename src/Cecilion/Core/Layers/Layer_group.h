#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "Layer.h"
namespace Cecilion {
    class Layer_stack;

    class Layer_group {
//        friend class Layer_stack<Events...>;
    public:

        /**
         * Append a new layer to the group. The layer will be put on top of the other layers,
         * which means all subscribed events will be directed to the new layer.
         * @tparam Events
         * @param layer
         */
        template<typename... Events>
        void append_layer(std::unique_ptr<Layer<Events...>> layer) {
            /// The user should give up ownership of the layer, but we need to share it internally.
            std::shared_ptr<Layer<Events...>> pointer = std::shared_ptr<Layer<Events...>>(layer.release());

            /** Call the quick fix function for all parameters. */
            (this->set_callback<Events>(pointer),...);
            this->m_layers.push_back(pointer);
            pointer->on_attach();
        }
        void hide() {this->visible = false;}
        void show() {this->visible = true;}
//    private:
        template<typename Event>
        bool try_forward(unsigned int event_ID) {
            if (!this->visible) {return false;}
            if (this->m_callback_mappings.find(typeid(Event)) != this->m_callback_mappings.end()) {
                std::static_pointer_cast<Layer<Event>>(this->m_callback_mappings[typeid(Event)])->template notify_me<Event>(event_ID);  // forward event to inbox.
                return true;
            }
            return false;
        }

        void on_update() {
            if (!this->visible) {return;}
            /** Reverse iterator */
            for (auto i = this->m_layers.rbegin();i != this->m_layers.rend();++i) {
                (*i)->on_update();
            }

        }

    private:

        /**
         * Apparently, lambda expressions can't capture parameter packs. This is used as a
         * quick fix for setting callbacks whenever a new layer is added.
         * @tparam Event
         * @tparam Events
         * @param layer
         */
        template<typename Event, typename... Events>
        void set_callback(std::shared_ptr<Layer<Events...>> layer) {
//            this->m_callback_mappings[typeid(Event)] = [layer](auto event_ID){layer->template notify_me<Event>(event_ID);};
            if (layer->template has_subscription<Event>()) {
                this->m_callback_mappings[typeid(Event)] = layer;
            }
        }
    private:
        std::vector<std::shared_ptr<I_Layer>> m_layers;
        std::unordered_map<std::type_index, std::shared_ptr<I_Layer>> m_callback_mappings;
        bool visible = true;
    };
}
