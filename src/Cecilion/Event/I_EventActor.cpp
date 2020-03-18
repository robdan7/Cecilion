//#include "EventSystem.h"

#include "I_EventActor.h"
#include "EventSystem.h"
using namespace Cecilion;
namespace Cecilion {

    I_EventActor::I_EventActor(std::string name) {
        this->ActorName = name;
    }

    I_EventActor::~I_EventActor() {

    }

    void I_EventActor::SubscribeTo(unsigned int MessageID, I_EventActor::EventCallback callback) {
        EventSystem::subscribe(MessageID, this, callback);
    }

    void I_EventActor::post(Cecilion::EventMessage *message) {
        EventSystem::post(message);
    }

    std::string I_EventActor::name() {
        return this->ActorName;
    }

    void I_EventActor::unsubscribe(unsigned int MessageID) {
        EventSystem::unsubscribe(MessageID, this);
    }
}
