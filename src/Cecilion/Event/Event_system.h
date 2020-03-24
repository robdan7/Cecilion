#pragma once
#include <map>
#include <list>
#include <iterator>
#include "Event_message.h"
#include "I_Event_actor.h"
//#include "Log.h"

using namespace std;


namespace Cecilion {
    class Event_message;
    class Event_system {
    public:
        static void post(const std::shared_ptr<Event_message>& message);
        static void post(unsigned int message_ID);
        static void Init();
        static void subscribe(unsigned int event_ID, const std::shared_ptr<Event_inbox>& inbox, Event_inbox::Event_callback callback);
        static void unsubscribe(unsigned int event_ID, Event_inbox* inbox);
        static void unsubscribe_all(const std::shared_ptr<Event_inbox>& inbox);
    private:
        static void dispatch_event(const std::shared_ptr<Event_message>& event);
    };


}
