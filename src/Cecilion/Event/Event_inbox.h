#pragma once
#include <map>
#include <list>
#include <mutex>
#include <thread>
#include "Event_message.h"
#include <condition_variable>
#include <utility>

namespace Cecilion {
    class I_Event_actor;
    class Event_inbox {
    public:

        /**
         * This is the function that will be called when a message is posted.
         */
        typedef void(*Event_callback)(std::shared_ptr<I_Event_actor> actor, Event_message* message);

        struct Buffered_event {
            Event_callback callback{};
            std::shared_ptr<Event_message> event;
            Buffered_event(std::shared_ptr<Event_message> message, Event_callback event_callback) {
                this->event = std::move(message); this->callback = event_callback;
            }
        };
        explicit Event_inbox(std::shared_ptr<I_Event_actor>  event_parent);
        ~Event_inbox();
        void send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback);
        static void post(const std::shared_ptr<Event_message>& message);
        void subscribe_to(int message_ID, Event_callback callback);
        void unsubscribe(int message_ID);
        void unsubscribe_all();
        std::shared_ptr<I_Event_actor> get_parent();
    private:
        std::list<int> subscribed_messages;
        std::shared_ptr<I_Event_actor>  p_event_parent;
        std::list<Buffered_event>* message_stack;
        std::mutex message_stack_m, event_thread_m;
        std::thread* event_thread{};

        void StartThread();
    };

    /**
 * Contains a callback function that will be used by the inbox when an
 * event arrives.
 */
    class Inbox_entry {
    public:
        Inbox_entry(std::shared_ptr<Event_inbox>  inbox, Event_inbox::Event_callback callback);
        std::shared_ptr<Event_inbox> get_inbox_target();
        void invoke_inbox(std::shared_ptr<Event_message>  event);
        bool operator ==(const Inbox_entry& entry) {
            return this->p_inbox == entry.p_inbox && this->callback == entry.callback;
        };
    private:
        const Event_inbox::Event_callback callback;
        std::shared_ptr<Event_inbox>  p_inbox;
    };
}
