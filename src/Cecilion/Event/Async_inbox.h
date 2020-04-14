#pragma once
#include <map>
#include <list>
#include <mutex>
#include <thread>
#include "Event_message.h"
#include "I_Event_inbox.h"
#include <condition_variable>
#include <utility>

namespace Cecilion {

    /**
     * Asynchronous inbox that will execute events in its own thread as soon as possible.
     * There is a chance that events will be put in a queue if a lot of them arrive at the same time,
     * but the inbox will never stall other parts of the system and will always execute events in order
     * from oldest to newest.
     */
    class Async_inbox : public I_Event_inbox {
    public:

        /**
         * This is the function that will be called when a message is posted.
         */
        //typedef void(*Event_callback)(std::shared_ptr<I_Event_actor> actor, Event_message* message);

        struct Buffered_event {
            Event_callback callback{};
            std::shared_ptr<Event_message> event;
            Buffered_event(std::shared_ptr<Event_message> message, Event_callback event_callback) {
                this->event = std::move(message); this->callback = event_callback;
            }
        };
        explicit Async_inbox(std::shared_ptr<I_Event_actor> event_parent);
        ~Async_inbox();
        void send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) override;
        //static void post(const std::shared_ptr<Event_message>& message);
        void subscribe_to(std::type_index message_ID, Event_callback callback) override;
        void unsubscribe(std::type_index message_ID) override;
        void unsubscribe_all();
    private:
        std::list<std::type_index> subscribed_messages;
        //std::shared_ptr<I_Event_actor>  p_event_parent;
        std::list<Buffered_event> p_message_stack;
        std::mutex message_stack_m, event_thread_m;
        std::thread* p_event_thread{};

        void StartThread();
    };

    /**
 * Contains a callback function that will be used by the inbox when an
 * event arrives.
 */
//    class Inbox_entry {
//    public:
//        Inbox_entry(std::shared_ptr<Async_inbox>  inbox, Async_inbox::Event_callback callback);
//        std::shared_ptr<Async_inbox> get_inbox_target();
//        void invoke_inbox(std::shared_ptr<Event_message>  event);
//        bool operator ==(const Inbox_entry& entry) {
//            return this->p_inbox == entry.p_inbox && this->callback == entry.callback;
//        };
//    private:
//        const Async_inbox::Event_callback callback;
//        std::shared_ptr<Async_inbox>  p_inbox;
//    };
}
