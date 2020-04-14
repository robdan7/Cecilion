#pragma once

#include "I_Event_inbox.h"

/**
 * A buffered inbox is just an inbox that stores all events in a buffer that must be
 * checked to dispatch all events. I.e. it is up to the event actor to dispatch its own incoming events.
 */
namespace Cecilion {
    class Buffered_inbox : public I_Event_inbox {
    public:
        Buffered_inbox(std::shared_ptr<I_Event_actor> event_parent);
        ~Buffered_inbox();
        void send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) override ;
        void subscribe_to(std::type_index message_ID, Event_callback callback) override;
        void unsubscribe(std::type_index message_ID) override;
        void unsubscribe_all();
        void execute_inbox();
    private:
        struct Buffered_event {
            Event_callback callback{};
            std::shared_ptr<Event_message> event;
            Buffered_event(std::shared_ptr<Event_message> message, Event_callback event_callback) {
                this->event = std::move(message); this->callback = event_callback;
            }
        };
        std::mutex message_stack_m;
        std::list<std::type_index> subscribed_messages;
        std::list<Buffered_event> p_message_stack;
    };
}


