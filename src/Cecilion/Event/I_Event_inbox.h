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

    /**
     * Generic class for event inboxes. We don't really care about HOW events are stored in
     * inboxes, all the event system should do is put events in an actors inbox and leave it there.
     */
    class I_Event_inbox {
    public:

        /**
         * This is the function that will be called when a message is posted.
         */
        typedef void(*Event_callback)(std::shared_ptr<I_Event_actor> actor, Event_message* message);

        explicit I_Event_inbox(std::shared_ptr<I_Event_actor>  event_parent);
        virtual void send_to_inbox(std::shared_ptr<Event_message> event, Event_callback callback) = 0;
        virtual void subscribe_to(int message_ID, Event_callback callback) = 0;
        virtual void unsubscribe(int message_ID) = 0;

        virtual std::shared_ptr<I_Event_actor> get_parent();
    protected:
        std::shared_ptr<I_Event_actor>  p_event_parent;
    };

    /**
* Contains a callback function that will be used by the inbox when an
* event arrives.
*/
    class Inbox_entry {
    public:
        Inbox_entry(std::shared_ptr<I_Event_inbox>  inbox, I_Event_inbox::Event_callback callback);
        std::shared_ptr<I_Event_inbox> get_inbox_target();
        void invoke_inbox(std::shared_ptr<Event_message>  event);
        bool operator ==(const Inbox_entry& entry) {
            return this->p_inbox == entry.p_inbox && this->callback == entry.callback;
        };
    private:
        const I_Event_inbox::Event_callback callback;
        std::shared_ptr<I_Event_inbox>  p_inbox;
    };
}
