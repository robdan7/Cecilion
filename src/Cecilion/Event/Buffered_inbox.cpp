//
// Created by Robin on 2020-03-26.
//

#include "Buffered_inbox.h"
#include "Event_system.h"
#include <Core/Log.h>
namespace Cecilion {
    void Buffered_inbox::unsubscribe(std::type_index message_ID) {
        Event_system::unsubscribe(message_ID, this);
        this->subscribed_messages.remove(message_ID);
        auto it = this->p_message_stack.begin();

        /// Delete all messages that are no longer in the subscription list.
        /// This stops the actor from executing messages that have arrived but should be discarded.
        int i = 0;
        while(it != this->p_message_stack.end()) {
            if ((*it).event->message_ID == message_ID) {
                it = this->p_message_stack.erase(it);
                i++;
            } else {
                ++it;
            }
        }
    }

    void Buffered_inbox::subscribe_to(std::type_index message_ID, I_Event_inbox::Event_callback callback) {
        this->subscribed_messages.push_back(message_ID);
        Event_system::subscribe(message_ID, std::shared_ptr<Buffered_inbox>(this), callback);
    }

    void Buffered_inbox::send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) {
        Buffered_inbox::Buffered_event buffered_event = Buffered_event(std::move(event), callback);
        this->message_stack_m.lock();
        this->p_message_stack.push_back(buffered_event);
        //this->condition.notify_one();
        this->message_stack_m.unlock();
    }

    Buffered_inbox::~Buffered_inbox() {
        this->unsubscribe_all();
//        delete this->p_message_stack;
    }

    void Buffered_inbox::unsubscribe_all() {
        // TODO unsibscribe all
    }

    /**
     * Execute all messages that are in the stack.
     */
    void Buffered_inbox::execute_inbox() {
        this->message_stack_m.lock();
        if (this->p_message_stack.size() == 0) {
            this->message_stack_m.unlock();
            return;
        }
        for (Buffered_event event : this->p_message_stack) {
            event.callback(event.event.get());
        }
        this->p_message_stack.clear();
        this->message_stack_m.unlock();
    }

    Buffered_inbox::Buffered_inbox(std::shared_ptr<I_Event_actor> event_parent) : I_Event_inbox(event_parent) {
//        this->p_message_stack = std::list<Buffered_event>();
    }
}

