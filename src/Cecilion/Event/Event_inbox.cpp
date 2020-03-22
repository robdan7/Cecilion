#include "Event_inbox.h"

#include <utility>
#include "Event_system.h"
#include "Log.h"
using namespace Cecilion;

Cecilion::Event_inbox::Event_inbox(std::shared_ptr<I_Event_actor> event_parent) {
    this->message_stack = new std::list<Buffered_event>();
    this->p_event_parent = std::move(event_parent);
}

Event_inbox::~Event_inbox() {
    this->unsubscribe_all();
        this->message_stack_m.lock();
    // Clearing the message stack will terminate the event thread if it is running.

    this->message_stack->clear();
    this->message_stack_m.unlock();
    if (this->event_thread->joinable()) {
        // Let the thread finish if it needs to run. It will quit immediately
        // because it's outside the loop and the message stack
        this->event_thread->join();
    }
}
/**
 * Start a thread that will execute events for an actor until there are no more events.
 * @param message_actor
 */
void Cecilion::Event_inbox::StartThread() {
        // Create the thread that takes care of all the event handling.
        this->event_thread = new std::thread ([this] () {
            this->event_thread_m.lock();

            bool condition = true;
            while (condition) { // Run until all events are dispatched.

                this->message_stack_m.lock();

                if (this->message_stack->empty()) {
                    this->message_stack_m.unlock();
                    condition = false;
                    continue;
                }

                // Get the event out of the stack.
                Buffered_event event_copy = this->message_stack->back();
                this->message_stack->pop_back();  // This will delete it from object memory.
                this->message_stack_m.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                event_copy.callback(this->p_event_parent, event_copy.event.get());
            }
            this->event_thread_m.unlock();
        });

        this->event_thread->detach();
}

/**
 * Add an event to the inbox. The event will be added to the inbox event buffer and executed
 * when the inbox has time for it. Others events will not block this method, since all events are executed
 * in a separate thread.
 *
 * @param event
 * @param callback
 */
void Event_inbox::send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) {
    Event_inbox::Buffered_event buffered_event = Buffered_event(std::move(event), callback);
    this->message_stack_m.lock();
    if (this->message_stack->empty()) {

        if (this->event_thread_m.try_lock()) {
            this->StartThread();
            this->event_thread_m.unlock();
        }
    }
    this->message_stack->push_front(buffered_event);
    //this->condition.notify_one();
    this->message_stack_m.unlock();
}

void Event_inbox::post(const std::shared_ptr<Event_message>& message) {
    Event_system::post(message);
}

/**
 * Subscribe to an event. Do not subscribe to an event twice.
 * @param message_ID
 * @param callback
 */
void Event_inbox::subscribe_to(int message_ID, Event_inbox::Event_callback callback) {
    // TODO add check for double subscription.
    this->message_stack_m.lock();
    Event_system::subscribe(message_ID, std::shared_ptr<Event_inbox>(this), callback);
    this->subscribed_messages.push_back(message_ID);
    this->message_stack_m.unlock();
}

/**
 * Unsubscribe from an event. Events that are still queued in the event list will still be executed,
 * but no new events will be posted to this inbox.
 * @param message_ID
 */
void Event_inbox::unsubscribe(int message_ID) {
    this->message_stack_m.lock();
    try {
        Event_system::unsubscribe(message_ID, this);
        this->subscribed_messages.remove(message_ID);
        auto it = this->message_stack->begin();

        /// Delete all messages that are no longer in the subscription list.
        int i = 0;
        while(it != this->message_stack->end()) {
            if ((*it).event->c_message_ID == message_ID) {
                it = this->message_stack->erase(it);
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

std::shared_ptr<I_Event_actor> Event_inbox::get_parent() {
    return this->p_event_parent;
}

void Event_inbox::unsubscribe_all() {
    //Event_system::unsubscribe_all(std::shared_ptr<Event_inbox>(this));
}

/// --- Inbox entry ---

std::shared_ptr<Event_inbox> Inbox_entry::get_inbox_target() {
    return this->p_inbox;
}

/**
 * Invoke the inbox that this entry is pointing at. This is all you need to deliver a message.
 * @param event
 */
void Inbox_entry::invoke_inbox(std::shared_ptr<Event_message>  event) {
    this->p_inbox->send_to_inbox(std::move(event), this->callback);
}

Inbox_entry::Inbox_entry(std::shared_ptr<Event_inbox>  inbox, Event_inbox::Event_callback callback) : callback(callback) {
    this->p_inbox = std::move(inbox);
}
