#pragma once
#include <map>
#include <list>
#include <iterator>
#include "Event_message.h"
#include "I_Event_actor.h"
#include "I_Event_inbox.h"
//#include "Log.h"

using namespace std;


namespace Cecilion {
    class Event_message;
    /**
     * The event system takes care of dispatching events and keeping track of which events should go where.
     * Whenever an event is dispatched, it is up to the actors that receive the event to properly
     * store or execute it. The event system is in no way responsible for lost events or stalling the rest of
     * the system because some actor wants to execute an event IMMEDIATELY on the same thread.
     *                                                          /\
     *                                                          | Highly discouraged.
     */
    class Event_system {
    public:
        static void post(const std::shared_ptr<Event_message>& message);
        static void post(unsigned int message_ID);
        static void Init();
        static void subscribe(unsigned int event_ID, const std::shared_ptr<I_Event_inbox>& inbox, I_Event_inbox::Event_callback callback);
        static void unsubscribe(unsigned int event_ID, I_Event_inbox* inbox);
        static void unsubscribe_all(const std::shared_ptr<I_Event_inbox>& inbox);
    private:
        static void dispatch_event(const std::shared_ptr<Event_message>& event);
    };


}
