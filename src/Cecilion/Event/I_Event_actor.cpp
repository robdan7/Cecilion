#include "I_Event_actor.h"
#include <utility>
#include "I_Event_inbox.h"

namespace Cecilion {

    I_Event_actor::I_Event_actor(std::string name) {
        this->actor_name = std::move(name);
    }

    I_Event_actor::~I_Event_actor() = default;

    void I_Event_actor::subscribe_to(std::type_index message_ID, I_Event_inbox::Event_callback callback) {
        this->actor_inbox.get()->subscribe_to(message_ID, callback);
    }


    std::string I_Event_actor::name() {
        return this->actor_name;
    }

    void I_Event_actor::unsubscribe(std::type_index message_ID) {
        this->actor_inbox.get()->unsubscribe(message_ID);
    }
    }
