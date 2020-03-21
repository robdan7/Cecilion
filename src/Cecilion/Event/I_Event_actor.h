#pragma once

#include <iostream>
#include "Event_message.h"

namespace Cecilion {

    /**
     * An event actor can both post and receive subscribed events from the event system.
     * When subscribing, a callback must be registered which will be used to call the actor
     * when an event has occurred. These types of function calls will always run in a separate thread.
     * It is therefore very important to secure the class for multi-threaded use.
     */
    class I_Event_actor {

    public:
        typedef void(*Event_callback)(I_Event_actor* actor, Event_message* message);
        I_Event_actor(std::string name);
        virtual ~I_Event_actor();
        void post(Event_message* message);
        void subscribe_to(unsigned int message_ID, Event_callback callback);
        void unsubscribe(unsigned int message_ID);
        std::string name();
        // TODO implement post with a time delay.
    private:
        std::string actor_name;
    };
}

