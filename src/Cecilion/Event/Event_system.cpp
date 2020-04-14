#include "Event_system.h"
#include "../Core/Log.h"

#include <mutex>
#include <utility>
#include <vector>
#include <thread>
#include "Core/Log.h"
#include "Inbox_entry_container.h"

namespace Cecilion {



    std::unordered_map<std::type_index, Inbox_entry_container*> Event_system::actor_list;
    std::mutex Event_system::actor_list_m;

/**
 * Subscribe to an event. When the event is posted a callback function will be called with the subscriber and
 * the message as a reference.
 *
 * One actor can have multiple entries for the same event, but the messaging system will always assume there
 * is only one entry per actor.
 * @param event_ID
 * @param actor
 * @param callback
 */
    void Cecilion::Event_system::subscribe(std::type_index event_ID, const std::shared_ptr<I_Event_inbox>& inbox, I_Event_inbox::Event_callback callback) {
        actor_list_m.lock();
        auto* actors = new Inbox_entry_container();
        if (actor_list.find(event_ID) == actor_list.end()) {
            actor_list[event_ID] = actors;
        } else {
            actors = actor_list[event_ID];
        }
        actors->add_inbox_entry(inbox, callback);
        actor_list_m.unlock();
        // TODO DELETE
        std::cout <<  event_ID.name() << std::endl;
        //CORE_LOG_TRACE("Actor " + inbox->get_parent()->name() + " subscribed to message ID " + std::to_string(message_ID));
    }

    void Event_system::unsubscribe(std::type_index event_ID, I_Event_inbox*  actor) {
        actor_list_m.lock();
        if (actor_list.count(event_ID) == 0) {
            CORE_LOG_ERROR("Delete exception. No actors are subscribed to {0}", event_ID.name());
            return;
        }
        actor_list.at(event_ID)->delete_entry(actor);
        actor_list_m.unlock();
    }

    void Event_system::unsubscribe_all(const std::shared_ptr<I_Event_inbox>& inbox) {
        // TODO implement this
    }


}
