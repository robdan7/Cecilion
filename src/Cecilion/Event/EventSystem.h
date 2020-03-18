#ifndef CECILION_EVENT_MESSAGINGSYSTEM_H
#define CECILION_EVENT_MESSAGINGSYSTEM_H
#include <map>
#include <list>
#include <iterator>
#include "EventMessage.h"
#include "I_EventActor.h"

using namespace std;


namespace Cecilion {
    class EventSystem {
    public:
        static void post(EventMessage* message);
        static void Init();
        static void subscribe(unsigned int messageID, I_EventActor* actor, I_EventActor::EventCallback callback);
        static void unsubscribe(unsigned int messageID, I_EventActor* actor);
    private:
        static void dispatchPost(EventMessage* message);
    };


}

#endif // CECILION_EVENT_MESSAGINGSYSTEM_H