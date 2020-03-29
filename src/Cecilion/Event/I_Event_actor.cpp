#include "I_Event_actor.h"
#include <utility>
#include "I_Event_inbox.h"

namespace Cecilion {

    I_Event_actor::I_Event_actor(std::string name) {

        //this->actor_inbox = new I_Event_inbox(std::shared_ptr<I_Event_actor>(this));

        //Cecilion::Async_inbox* t = new Cecilion::Async_inbox(this);
//        LOG_INFO(std::to_string(this->actor_inbox->test()));
        this->actor_name = std::move(name);
    }

    I_Event_actor::~I_Event_actor() = default;

    void I_Event_actor::subscribe_to(unsigned int message_ID, I_Event_inbox::Event_callback callback) {
        this->actor_inbox.get()->subscribe_to(message_ID, callback);
        //Event_system::subscribe(message_ID, this, callback);
    }


    std::string I_Event_actor::name() {
        return this->actor_name;
    }

    void I_Event_actor::unsubscribe(unsigned int message_ID) {
        this->actor_inbox.get()->unsubscribe(message_ID);
    }

//    I_Event_actor::I_Event_actor(std::string name, I_Event_inbox* inbox) {
//        this->actor_name = name;
//        this->actor_inbox = inbox;
//    }

//    void I_Event_actor::post(unsigned int message_ID) {
//        this->post(std::make_shared<Event_message>(message_ID));
//    }

}
