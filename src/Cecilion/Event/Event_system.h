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
        static void post(Event_message* message);
        static void Init();
        static void subscribe(unsigned int message_ID, I_Event_actor* actor, I_Event_actor::Event_callback callback);
        static void unsubscribe(unsigned int message_ID, I_Event_actor* actor);
    private:
        static void dispatch_event(Event_message* message);
    };


}
