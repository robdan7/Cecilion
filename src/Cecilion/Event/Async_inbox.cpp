#include "Async_inbox.h"

#include <utility>
#include "Event_system.h"
#include "Core/Log.h"
namespace Cecilion {
    Async_inbox::Async_inbox(std::shared_ptr<I_Event_actor> event_parent) : I_Event_inbox(event_parent){
        this->p_message_stack = new std::list<Buffered_event>();
        //this->p_event_parent = event_parent;
    }

    Async_inbox::~Async_inbox() {
        this->unsubscribe_all();
        this->message_stack_m.lock();
        // Clearing the message stack will terminate the event thread if it is running.

        this->p_message_stack->clear();
        this->message_stack_m.unlock();
        if (this->p_event_thread->joinable()) {
            // Let the thread finish if it needs to run. It will quit immediately
            // because it's outside the loop and the message stack
            this->p_event_thread->join();
        }
        delete this->p_message_stack;
        delete this->p_event_thread;
    }
/**
 * Start a thread that will execute events for an actor until there are no more events.
 * @param message_actor
 */
    void Async_inbox::StartThread() {
        // Create the thread that takes care of all the event handling.
        this->p_event_thread = new std::thread ([this] () {
            this->event_thread_m.lock();

            bool condition = true;
            while (condition) { // Run until all events are dispatched.

                this->message_stack_m.lock();

                if (this->p_message_stack->empty()) {
                    this->message_stack_m.unlock();
                    condition = false;
                    continue;
                }

                // Get the event out of the stack.
                Buffered_event event_copy = this->p_message_stack->front();
                this->p_message_stack->pop_front();  // This will delete it from object memory.
                this->message_stack_m.unlock();
                event_copy.callback(this->p_event_parent, event_copy.event.get());
            }
            this->event_thread_m.unlock();
        });

        this->p_event_thread->detach();
    }

/**
 * Add an event to the inbox. The event will be added to the inbox event buffer and executed
 * when the inbox has time for it. Others events will not block this method, since all events are executed
 * in a separate thread.
 *
 * @param event
 * @param callback
 */
    void Async_inbox::send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) {
        Async_inbox::Buffered_event buffered_event = Buffered_event(std::move(event), callback);
        this->message_stack_m.lock();
        if (this->p_message_stack->empty()) {

            if (this->event_thread_m.try_lock()) {
                this->StartThread();
                this->event_thread_m.unlock();
            }
        }
        this->p_message_stack->push_back(buffered_event);
        //this->condition.notify_one();
        this->message_stack_m.unlock();
    }

//    void Async_inbox::post(const std::shared_ptr<Event_message>& message) {
//        Event_system::post(message);
//    }

/**
 * Subscribe to an event. Do not subscribe to an event twice.
 * @param message_ID
 * @param callback
 */
    void Async_inbox::subscribe_to(int message_ID, Async_inbox::Event_callback callback) {
        // TODO add check for double subscription.
        this->message_stack_m.lock();
        Event_system::subscribe(message_ID, std::shared_ptr<Async_inbox>(this), callback);
        this->subscribed_messages.push_back(message_ID);
        this->message_stack_m.unlock();
    }

/**
 * Unsubscribe from an event. Events that are still queued in the event list will still be executed,
 * but no new events will be posted to this inbox.
 * @param message_ID
 */
    void Async_inbox::unsubscribe(int message_ID) {
        this->message_stack_m.lock();
        try {
            Event_system::unsubscribe(message_ID, this);
            this->subscribed_messages.remove(message_ID);
            auto it = this->p_message_stack->begin();

            /// Delete all messages that are no longer in the subscription list.
            int i = 0;
            while(it != this->p_message_stack->end()) {
                if ((*it).event->message_ID == message_ID) {
                    it = this->p_message_stack->erase(it);
                    i++;
                } else {
                    ++it;
                }
            }
            if (i) CORE_LOG_WARN("There were "+ std::to_string(i) +" events in inbox buffer when actor unsubscribed.");
        } catch (int e) {
            CORE_LOG_ERROR(" Event actor " + this->get_parent()->name() + "is not a subscriber to event ID " + std::to_string(message_ID));
        }
        this->message_stack_m.unlock();
    }

    void Async_inbox::unsubscribe_all() {
        // TODO unsubscribe all
        //Event_system::unsubscribe_all(std::shared_ptr<Async_inbox>(this));
    }

///// --- Inbox entry ---
//
//    std::shared_ptr<Async_inbox> Inbox_entry::get_inbox_target() {
//        return this->p_inbox;
//    }
//
///**
// * Invoke the inbox that this entry is pointing at. This is all you need to deliver a message.
// * @param event
// */
//    void Inbox_entry::invoke_inbox(std::shared_ptr<Event_message>  event) {
//        this->p_inbox->send_to_inbox(std::move(event), this->callback);
//    }
//
//    Inbox_entry::Inbox_entry(std::shared_ptr<Async_inbox>  inbox, Async_inbox::Event_callback callback) : callback(callback) {
//        this->p_inbox = std::move(inbox);
//    }
}


