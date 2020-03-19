//#include "Event_system.h"

#include "I_Event_actor.h"
#include "Event_system.h"
using namespace Cecilion;
namespace Cecilion {

    I_Event_actor::I_Event_actor(std::string name) {
        this->actor_name = name;
    }

    I_Event_actor::~I_Event_actor() {

    }

    void I_Event_actor::subscribe_to(unsigned int message_ID, I_Event_actor::Event_callback callback) {
        Event_system::subscribe(message_ID, this, callback);
    }

    void I_Event_actor::post(Cecilion::Event_message *message) {
        Event_system::post(message);
    }

    std::string I_Event_actor::name() {
        return this->actor_name;
    }

    void I_Event_actor::unsubscribe(unsigned int message_ID) {
        Event_system::unsubscribe(message_ID, this);
    }
}
