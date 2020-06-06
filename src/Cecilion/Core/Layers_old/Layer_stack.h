#pragma once

#include <Event/Event_actor_st.h>
#include <vector>
#include <list>
#include "Application_layer.h"


namespace Cecilion {
//    class Application_layer_st;   TODO do I need this?


    /**
     * The layer stack contains multiple layers that can subscribe to
     * the predefined events within the stack. The top most layer will always
     * receive the event and block all other layers from receiving the event.
     *
     * Layers are also divided into normal layers and overlays.
     * @tparam Events
     */
    template<typename... Events>
    class Layer_stack : public Event_actor_st<Events...> {
        // TODO Create the code for grouping layers so we don't need to predefine overlays.
        // TODO make this thread safe.
    public:
        Layer_stack();
////        static void event_inbox(std::shared_ptr<I_Event_actor> actor, Event_message* event);
////        void dispatch_layer_event(const std::shared_ptr<Event_message>&  event);
//        template<typename Event>
//        void add_layer_subscription();

        template<typename... Layer_event>
        void push_layer(const std::shared_ptr<Application_layer<Layer_event...>>& layer, bool overlay);
        void pop_layer(const std::shared_ptr<Application_layer<void>>& layer, bool overlay);
        void on_update();
//        void on_imgui_render();
    private:
        void remap_inbox_entries(Application_layer<void>* layer);
//        std::map<unsigned int, Inbox_entry*> inbox_entries;
        std::list<std::shared_ptr<Application_layer<void>>> layers;
        std::list<std::shared_ptr<Application_layer<void>>> overlays;

        std::tuple<std::shared_ptr<Application_layer<Events>>...> top_overlays;
        std::tuple<std::shared_ptr<Application_layer<Events>>...> top_layers;
//        std::list<std::type_index> subscribed_events;
    };



    template<typename... Events>
    Layer_stack<Events...>::Layer_stack() {
        this->layers = std::list<std::shared_ptr<Application_layer<void>>>();
        this->overlays = std::list<std::shared_ptr<Application_layer<void>>>();

        // TODO it fails here, and I dunno why.
        (this->template set_callback<Events>([this](auto event){
            auto layer = std::get<std::shared_ptr<Application_layer<Events>>>(this->top_overlays);
            if (layer != nullptr) {
                layer->template notify_event<Events>(event);
            } else if (std::get<std::shared_ptr<Application_layer<Events>>>(this->top_layers) != nullptr) {
                std::get<std::shared_ptr<Application_layer<Events>>>(this->top_overlays)-> template notify_event<Events>(event);
            }
//                if (!this->overlays.empty()) {
//                    this->overlays.back().get()->dispatch(event);
//                } else if(!this->layers.empty()) {
//                    this->layers.back().get()->dispatch(event);
//                }
        }),...);
        

    }

    template<typename... Events>
    void Layer_stack<Events...>::pop_layer(const std::shared_ptr<Application_layer<void>> &layer, bool overlay) {
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

    /**
     * Push a layer to the stack. All layers that do not wish to become an overlay will be put behind
     * the first overlay. Every overlay after the fist one will be put on the top of the stack.
     * @param layer
     * @param overlay
     */
    template<typename... Events>
    template<typename... Layer_event>
    void Layer_stack<Events...>::push_layer(const std::shared_ptr<Application_layer<Layer_event...>> &layer, bool overlay) {
        // Note: end -1 returns the LAST element, not the element before the last one.
        // end points to the theoretical element after the last one.

        if (overlay) {

//            if (!this->overlays.empty()) {
//                layer->set_previous(this->overlays.back().get());
//            } else if (!this->layers.empty()) {
//                layer->set_previous(this->overlays.front().get());
//            }
            this->overlays.push_back(layer);

            /** This should replace which overlay to notify for events.**/
            ([this, layer](){
                std::get<std::shared_ptr<Application_layer<Events>>>(this->top_overlays) =
                        std::static_pointer_cast<Application_layer<Events>>(layer);
            },...);

        } else {
//            if (!this->overlays.empty()) {
//                this->overlays.front()->set_previous(layer.get());
//            }
//            if (!this->layers.empty()) {
//                layer->set_previous(this->layers.back().get());
//            }
            this->layers.push_back(layer);

            ([this, layer](){
                std::get<std::shared_ptr<Application_layer<Events>>>(this->top_layers) =
                        std::static_pointer_cast<Application_layer<Events>>(layer);
            },...);

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


//        layer->attach_to_stack(this);
        layer->on_attach(); // TODO set parent to this.
    }

    template<typename... Events>
    void Layer_stack<Events...>::on_update() {

        /** The following lambda functions will dispatch any new events.
         *  Remember that events will be cached in the top most layer, which is either a layer
         *  or an overlay, so there is no need to go through all layers to see if they are subscribed
         *  to an event or not.
         * **/
        ([this](){
            if (std::get<std::shared_ptr<Application_layer<Events>>>(this->top_overlays) != nullptr) {
                std::get<std::shared_ptr<Application_layer<Events>>>(this->top_overlays)->check_inbox();
            }
            },...);
        ([this](){
            if (std::get<std::shared_ptr<Application_layer<Events>>>(this->top_layers) != nullptr) {
                std::get<std::shared_ptr<Application_layer<Events>>>(this->top_layers)->check_inbox();
            }
        },...);
        for (const std::shared_ptr<Application_layer<void>>& layer : this->layers) {
            layer->on_update();
        }
        for (const std::shared_ptr<Application_layer<void>>& layer : this->overlays) {
            layer->on_update();
        }
        //this->layers->front()->on_update();
    }

}

