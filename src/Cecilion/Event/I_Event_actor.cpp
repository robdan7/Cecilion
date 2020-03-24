#include "I_Event_actor.h"
#include <utility>
#include "Event_inbox.h"

namespace Cecilion {

    I_Event_actor::I_Event_actor(std::string name) {
        this->actor_inbox = new Event_inbox(std::shared_ptr<I_Event_actor>(this));

        //Cecilion::Event_inbox* t = new Cecilion::Event_inbox(this);
//        LOG_INFO(std::to_string(this->actor_inbox->test()));
        this->actor_name = std::move(name);
    }

    I_Event_actor::~I_Event_actor() = default;

    void I_Event_actor::subscribe_to(unsigned int message_ID, Event_inbox::Event_callback callback) {
        this->actor_inbox->subscribe_to(message_ID, callback);
        //Event_system::subscribe(message_ID, this, callback);
    }

    /**
     * Post an event from the event actor. The message pointer can be discarded afterwards, since
     * a local reference will be kept in the actor inbox until the event is executed.
     *
     * @param message
     */
    void I_Event_actor::post(std::shared_ptr<Event_message> message) {
        this->actor_inbox->post(std::move(message));
        //Event_system::post(message);
    }

    std::string I_Event_actor::name() {
        return this->actor_name;
    }

    void I_Event_actor::unsubscribe(unsigned int message_ID) {
        this->actor_inbox->unsubscribe(message_ID);
    }

}
