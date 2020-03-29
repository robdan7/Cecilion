#pragma once

#include <iostream>
#include "Event_message.h"
#include "I_Event_inbox.h"

namespace Cecilion {

    /**
     * An event actor can both acts on subscribed events from the event system, i.e. it gets notified
     * by the event system when something relevant has happened.
     *
     * When subscribing, a callback must be registered which will be used to call the actor
     * when an event has occurred. These types of function calls will always run in a separate thread.
     * It is therefore very important to secure the class for multi-threaded use.
     */
    class I_Event_actor {
    public:
        //typedef void(*Event_callback)(I_Event_actor* actor, Event_message* message);

        virtual ~I_Event_actor();
        std::string name();
        // TODO implement post with a time delay.
    private:
        std::string actor_name;
    protected:
        std::shared_ptr<I_Event_inbox> actor_inbox;
        I_Event_actor(std::string name);
        //I_Event_actor(std::string name, I_Event_inbox* inbox);
        virtual void subscribe_to(unsigned int message_ID, I_Event_inbox::Event_callback callback);
        virtual void unsubscribe(unsigned int message_ID);
    };
}

