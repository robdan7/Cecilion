#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "Layer.h"
#include <Core/Log.h>
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

            /** Call the quick fix function for all event types. */
            (this->set_callback<Events>(pointer),...);
            this->m_layers.push_back(pointer);
            pointer->on_attach();
        }
        void hide() {this->visible = false;}
        void show() {this->visible = true;}

        /**
         * Try to handle an event.
         * @tparam Event - the event type
         * @param event_ID - The unique event ID.
         * @return - The function returns true only if the event was taken care of.
         */
        template<typename Event>
        bool try_forward(unsigned int event_ID) {
            if (!this->visible) {return false;}
            if (this->m_callback_functions.find(typeid(Event)) != this->m_callback_functions.end()) {
//                this->m_callback_functions[typeid(Event)](event_ID);
                this->m_callback_functions[typeid(Event)].callback(event_ID, this->m_callback_functions[typeid(Event)].layer);
//                std::static_pointer_cast<Layer<Event>>(this->m_callback_mappings[typeid(Event)])->template notify_me<Event>(event_ID);  // forward event to inbox.
                return true;
            }
            return false;
        }

        /**
         * Update all layers from front to back.
         */
        void on_update() {
            if (!this->visible) {return;}
            for (auto& layer: this->m_layers) {
                layer->on_update();
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
//                this->m_callback_mappings[typeid(Event)] = layer;
            this->m_callback_functions[typeid(Event)] = {layer,
                [](unsigned int event, std::shared_ptr<I_Layer> layer){
                    std::static_pointer_cast<Layer<Events...>>(layer)->template notify_me<Event>(event);  // forward event to inbox.
                }
            };
//            std::weak_ptr<Layer<Events...>> pointer(layer);
//            this->m_callback_functions[typeid(Event)] = [pointer]<Events>(unsigned int event){
//                auto object = pointer.lock();
//                if (object) {
//                    object->template notify<Events...>(event);
//                }
////                    std::static_pointer_cast<Layer<Events...>>(this->m_layers[size])-> template notify_me<Event>(event);  // forward event to inbox.
//            };
        }
    private:
        struct Callback_container {
                    std::shared_ptr<I_Layer> layer;
                    std::function<void(unsigned int, std::shared_ptr<I_Layer>)> callback;
                };
        std::vector<std::shared_ptr<I_Layer>> m_layers;
//        std::unordered_map<std::type_index, std::shared_ptr<I_Layer>> m_callback_mappings;
        std::unordered_map<std::type_index, Callback_container> m_callback_functions;
        bool visible = true;
    };
}
