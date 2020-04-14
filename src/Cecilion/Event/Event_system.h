#pragma once
#include <map>
#include <list>
#include <iterator>
#include "Event_message.h"
#include "I_Event_actor.h"
#include "I_Event_inbox.h"
#include "Inbox_entry_container.h"
//#include "../Core/Log.h"
//#include <Core/Log.h>

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
//        static void Init();

        template<typename Event, typename... Args>
        static void post(Args... args) {
            dispatch_event<Event>(std::make_shared<Event>(args...));
        }

        template<typename Event>
        static void post() {
            dispatch_event<Event>(std::make_shared<Event>());
        }

        static void post(std::type_index event_ID) {
            dispatch_event<Event_message>(std::make_shared<Event_message>(event_ID));
        }


        static void subscribe(std::type_index event_ID, const std::shared_ptr<I_Event_inbox>& inbox, I_Event_inbox::Event_callback callback);
        static void unsubscribe(std::type_index event_ID, I_Event_inbox* inbox);
        static void unsubscribe_all(const std::shared_ptr<I_Event_inbox>& inbox);
    private:
        /**
        * This sends an event
        * @param event
        */
        template<typename Event>
        static void dispatch_event(std::shared_ptr<Event> event) {
            /// The following line is somewhat of a hack. The typeid of a shared pointer is not the same
            /// as a the type of a normal object. We therefore need to reset the ID to the actual type of the event.
            // TODO Do something so this line can be removed completely.
            ((Event*)event.get())->message_ID = typeid(Event);
            actor_list_m.lock();
            if (actor_list.count(typeid(Event))) {
                try {
                    actor_list.at(typeid(Event))->dispatch(static_cast<std::shared_ptr<Event_message>>(event));
                    actor_list_m.unlock();
                    return;
                } catch(int e) {
                    /// There is a list of inboxes, but does no longer contain any inbox entries.
                    actor_list.erase(typeid(Event));
                }
            }
//            CORE_LOG_TRACE("No actor is currently listening to message ID {0} ", event->message_ID.name());
            actor_list_m.unlock();
        }

    private:
        static std::unordered_map<std::type_index, Inbox_entry_container*> actor_list;
        static std::mutex actor_list_m;
        static bool initialized;
    };


}
