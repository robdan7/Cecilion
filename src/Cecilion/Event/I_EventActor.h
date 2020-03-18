//
// Created by Robin on 2020-03-17.
//

#ifndef CECILION_I_EVENTACTOR_H
#define CECILION_I_EVENTACTOR_H

#include <iostream>
#include "EventMessage.h"

namespace Cecilion {

    /**
     * An event actor can both post and receive subscribed events from the event system.
     * When subscribing, a callback must be registered which will be used to call the actor
     * when an event has occurred. These types of function calls will always run in a separate thread.
     * It is therefore very important to secure the class for multi-threaded use.
     */
    class I_EventActor {

    public:
        typedef void(*EventCallback)(I_EventActor* actor, EventMessage* message);
        I_EventActor(std::string name);
        virtual ~I_EventActor();
        void post(EventMessage* message);
        void SubscribeTo(unsigned int MessageID, EventCallback callback);
        void unsubscribe(unsigned int MessageID);
        std::string name();
        // TODO implement post with a time delay.
    private:
        std::string ActorName;
    };
}

#endif //CECILION_I_EVENTACTOR_H
