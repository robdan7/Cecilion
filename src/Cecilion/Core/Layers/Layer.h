#pragma once
#include <iostream>
#include <Event/Event_actor.h>
namespace Cecilion {

    class I_Layer {
    public:
        virtual void on_update() = 0;
        virtual void on_attach() = 0;
    };

    template<typename... Events>
    class Layer : public I_Layer, public Event_actor<Events...>{
    public:
//        void on_update() override;

        Layer();
        template<typename Event>
        void notify_me(unsigned int event_ID);
        void on_update() override;
        void on_attach() override {}
    };

    template<typename... Events>
    template<typename Event>
    void Layer<Events...>::notify_me(unsigned int event_ID) {
        this->template notify<Event>(event_ID);
    }

    template<typename... Events>
    Layer<Events...>::Layer() {

    }

    template<typename... Events>
    void Layer<Events...>::on_update() {
        this->check_inbox();
    }
}
