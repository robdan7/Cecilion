#include "I_Event_inbox.h"
#include <Core/Log.h>
namespace Cecilion {
    Cecilion::I_Event_inbox::I_Event_inbox(std::shared_ptr<I_Event_actor> event_parent) {
        this->p_event_parent = event_parent;
    }

    std::shared_ptr<Cecilion::I_Event_actor> Cecilion::I_Event_inbox::get_parent() {
        return this->p_event_parent;
    }

/// --- Inbox entry ---

    std::shared_ptr<I_Event_inbox> Inbox_entry::get_inbox_target() {
        return this->p_inbox;
    }

/**
 * Invoke the inbox that this entry is pointing at. This is all you need to deliver a message.
 * @param event
 */
    void Inbox_entry::invoke_inbox(std::shared_ptr<Event_message>  event) {
        this->p_inbox->send_to_inbox(std::move(event), this->callback);
    }

    Inbox_entry::Inbox_entry(std::shared_ptr<I_Event_inbox>  inbox, I_Event_inbox::Event_callback callback) : callback(callback) {
            this->p_inbox = std::move(inbox);
    }
}


