#pragma once

#include "Event_actor.h"
namespace Cecilion {


    template<typename... Events>
    class Event_actor_mt : Event_actor<Events...> {
    public:
        Event_actor_mt() : Event_actor<Events...>() {
            this->start_thread();
            this->init_subscriptions();
        }
    private:
        void start_thread() {

        }
    };
}