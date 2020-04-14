//
// Created by Robin on 2020-03-25.
//

#include "Layer_stack.h"
#include "algorithm"
#include <Event/Async_inbox.h>
#include <Core/Log.h>
#include "Application_layer_st.h"

namespace Cecilion {


    Layer_stack::Layer_stack() : I_Event_actor("") {
        this->actor_inbox = std::make_shared<Async_inbox>(std::shared_ptr<I_Event_actor>(this));
        this->layers = std::list<std::shared_ptr<Application_layer>>();
        this->overlays = std::list<std::shared_ptr<Application_layer>>();
    }

    /**
    * This should be used by layers when they want to subscibe to an event.
    * The layer stack will call the top most layer for all events, and that layer will call the second one
    * and so on. If a layer has subscribed to and event, it will stop the recursive call pattern and
    * handle the event.
    * @param event_ID
    */
    void Layer_stack::add_layer_subscription(std::type_index event_ID) {
        auto it = std::find(this->subscribed_events.begin(), this->subscribed_events.end(), event_ID);
        if (it == this->subscribed_events.end()) {
            /// only add a subscription if we haven't subscribed yet.
            this->subscribed_events.push_back(event_ID);

            /// Add subscription to the event system.
            this->subscribe_to(event_ID, [this](Event_message *event){
//                CORE_LOG_INFO("Dispatching event {0} to overlays", event->message_ID.name());
                if (!this->overlays.empty()) {
                    //CORE_LOG_INFO("Dispatching event {0} to overlays", event->message_ID);
                    this->overlays.back().get()->dispatch(std::shared_ptr<Event_message>(event));
                } else if(!this->layers.empty()) {
                    //CORE_LOG_INFO("Dispatching event {0} to layers", event->message_ID);
                    this->layers.back().get()->dispatch(std::shared_ptr<Event_message>(event));
                }
            });
        }
    }

/**
 * Push a layer to the stack. Alla layers that do not wish to become an overlay will be put behind
 * the first overlay. Every overlay after the fist one will be put on the top of the stack.
 * @param layer
 * @param overlay
 */
    void Layer_stack::push_layer(const std::shared_ptr<Application_layer>& layer, bool overlay) {
        // Note: end -1 returns the LAST element, not the element before the last one.
        // end points to the theoretical element after the last one.

        if (overlay) {

            if (!this->overlays.empty()) {
                layer->set_previous(this->overlays.back().get());
            } else if (!this->layers.empty()) {
                layer->set_previous(this->overlays.front().get());
            }
            this->overlays.push_back(layer);

        } else {
            if (!this->overlays.empty()) {
                this->overlays.front()->set_previous(layer.get());
            }
            if (!this->layers.empty()) {
                layer->set_previous(this->layers.back().get());
            }
            this->layers.push_back(layer);

//
//            if (this->layers.begin() != this->bottom_overlay) {
//                layer->set_previous((this->bottom_overlay - 1)->get());
//            }
//            if (this->bottom_overlay != this->layers.end()) {
//                this->bottom_overlay->get()->set_previous(layer.get());
//            }
//            // TODO this is not working.
////            if (this->layers.size() > 0) {
////                layer->set_previous((this->bottom_overlay - 1)->get());
////            }
//            this->layers.insert(this->bottom_overlay, layer);
//            //this->bottom_overlay->get()->set_previous(layer.get());
        }
        layer->attach_to_stack(this);
        layer->on_attach();
    }

/**
 * Effectively delete a layer from the stack. The layer itself will not be deleted if a shared pointer
 * is kept somewhere else in memory.
 * @param layer
 * @param overlay
 */
    void Layer_stack::pop_layer(const std::shared_ptr<Application_layer>& layer, bool overlay) {
        // TODO implement a working method.
//    if (layer.get() == this->bottom_overlay->get()) {
//
//    }
//        std::list<std::shared_ptr<Application_layer_st>>::iterator it = this->layers.begin();
//        while (it->get() != layer.get()) {
//            if (it == this->layers.end()) {
//                // TODO Throw exception.
//                return;
//            }
//            it++;
//        }
//        if (it->get() == this->bottom_overlay->get()) {
//            this->bottom_overlay = it + 1;
//        }
//        if ((it + 1) != this->layers.end()) {
//            (it + 1)->get()->set_previous((it - 1)->get());
//        }
//        this->layers.erase(it);
    }

    void Layer_stack::on_update() {
        for (const std::shared_ptr<Application_layer>& layer : this->layers) {
            layer->on_update();
        }
        for (const std::shared_ptr<Application_layer>& layer : this->overlays) {
            layer->on_update();
        }
        //this->layers->front()->on_update();
    }

    void Layer_stack::remap_inbox_entries(Application_layer* layer) {

    }

    void Layer_stack::on_imgui_render() {
        for (const std::shared_ptr<Application_layer>& layer : this->layers) {
            layer->on_imgui_render();
        }
        for (const std::shared_ptr<Application_layer>& layer : this->overlays) {
            layer->on_imgui_render();
        }
    }

}