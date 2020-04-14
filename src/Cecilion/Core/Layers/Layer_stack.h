#pragma once

#include <Event/I_Event_actor.h>
#include <vector>
#include "Application_layer.h"

namespace Cecilion {
    class Application_layer_st;
    class Layer_stack : public I_Event_actor {
    public:
        Layer_stack();
//        static void event_inbox(std::shared_ptr<I_Event_actor> actor, Event_message* event);
//        void dispatch_layer_event(const std::shared_ptr<Event_message>&  event);
        void add_layer_subscription(std::type_index event_ID);
        void push_layer(const std::shared_ptr<Application_layer>& layer, bool overlay);
        void pop_layer(const std::shared_ptr<Application_layer>& layer, bool overlay);
        void on_update();
        void on_imgui_render();
    private:
        void remap_inbox_entries(Application_layer* layer);
        std::map<unsigned int, Inbox_entry*> inbox_entries;
        std::list<std::shared_ptr<Application_layer>> layers;
        std::list<std::shared_ptr<Application_layer>> overlays;
        std::list<std::type_index> subscribed_events;
    };
}

