#pragma once
#include "Event_actor.h"

/**
 * Single threaded version of an event actor. The inbox must be checked manually
 * for events to be dispatched. Use this for synchronized execution of events.
 *
 * Also make sure to never leave a single threaded actor without checking events,
 * since this will clog up the event system with dormant events that are never executed.
 */
namespace Cecilion {

    template<typename... Events>
    class Event_actor_st : public Event_actor<Events...> {
    public:

        Event_actor_st() : Event_actor<Events...>() {
            this->init_subscriptions();
        }

        /**
         * Just an update method. This is all you need to call in order to execute cached events.
         */
        void on_update() {
            this->check_inbox();
        }
    };
}


