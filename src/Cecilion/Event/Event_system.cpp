#include "Event_system.h"
//#include "../Core/Log.h"

#include <mutex>
#include <utility>
#include <vector>
#include <thread>
#include "Core/Log.h"

namespace Cecilion {

    class ActorEntryContainer {
        list<Inbox_entry*> inbox_entry_list;
        std::mutex inbox_entry_m;
    public:
        ~ActorEntryContainer() {
            this->inbox_entry_list.clear();
        }
        void add_inbox_entry(std::shared_ptr<Event_inbox> inbox, Event_inbox::Event_callback callback) {
            this->inbox_entry_m.lock();
            this->inbox_entry_list.push_front(new Inbox_entry(std::move(inbox), callback));
            this->inbox_entry_m.unlock();
        }

        void deleteActor(Event_inbox*  p_inbox) {

            this->inbox_entry_m.lock();
            Inbox_entry* search_entry;
            for (Inbox_entry* entry : this->inbox_entry_list) {
                if (entry->get_inbox_target().get() == p_inbox) {
                    search_entry = entry;
                }
            }
            if (search_entry != nullptr) {
                this->inbox_entry_list.remove(search_entry);
            } else {
                // TODO add proper throw event.
                throw -1;
            }
//        this->actors->remove(actor);
            this->inbox_entry_m.unlock();
        }

        void dispatch(const std::shared_ptr<Event_message>& event) {
            this->inbox_entry_m.lock();
            if (this->inbox_entry_list.empty()) {
                this->inbox_entry_m.unlock();
                // TODO add proper throw event.
                throw -1;
            }
            for (Inbox_entry* entry : this->inbox_entry_list) {
                entry->invoke_inbox(event);
            }
            this->inbox_entry_m.unlock();
        }
    };


    map<unsigned int, ActorEntryContainer*>* actor_list;
    std::mutex actor_list_m;
    bool initialized = false;

/**
 * Print an error message if the messaging system has not been intitialized yet.
 */
    void CheckInit() {
        if (!initialized) {
            CORE_LOG_ERROR("Messaging system is not initialized.");
        }
    }

    void Cecilion::Event_system::Init() {
        initialized = true;
        actor_list = new map<unsigned int, ActorEntryContainer*>();
        CORE_LOG_INFO("Event system up and running");
    }

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
    void Cecilion::Event_system::subscribe(unsigned int event_ID, const std::shared_ptr<Event_inbox>& inbox, Event_inbox::Event_callback callback) {
        CheckInit();
        actor_list_m.lock();
        auto* actors = new ActorEntryContainer();
        if (actor_list->find(event_ID) == actor_list->end()) {
            (*actor_list)[event_ID] = actors;
        } else {
            actors = (*actor_list)[event_ID];
        }
        actors->add_inbox_entry(inbox, callback);
        actor_list_m.unlock();
        //CORE_LOG_TRACE("Actor " + inbox->get_parent()->name() + " subscribed to message ID " + std::to_string(message_ID));
    }

    void Event_system::post(const std::shared_ptr<Event_message>& message) {
        CheckInit();
        dispatch_event(message);
    }

    void Event_system::post(unsigned int message_ID) {
        CheckInit();
        dispatch_event(std::make_shared<Event_message>(message_ID));
    }

/**
 * This sends an event
 * @param event
 */
    void Event_system::dispatch_event(const std::shared_ptr<Event_message>& event) {
        actor_list_m.lock();
        if (actor_list->count(event->c_message_ID) > 0) {
            try {
                actor_list->at(event->c_message_ID)->dispatch(event);
                actor_list_m.unlock();
                return;
            } catch(int e) {
                // There is a list of inboxes, but does no longer contain any inbox entries.
                actor_list->erase(event->c_message_ID);
            }

        }
        //CORE_LOG_ERROR("No actor is currently listening to message ID  " + std::to_string(event->c_message_ID));
        actor_list_m.unlock();
    }

    void Event_system::unsubscribe(unsigned int event_ID, Event_inbox*  actor) {
        CheckInit();
        actor_list_m.lock();
        if (actor_list->count(event_ID) == 0) {
            CORE_LOG_ERROR("Delete exception. No actors are subscribed to " + std::to_string(event_ID));
            return;
        }
        actor_list->at(event_ID)->deleteActor(actor);
        actor_list_m.unlock();
    }

    void Event_system::unsubscribe_all(const std::shared_ptr<Event_inbox>& inbox) {
        // TODO implement this
    }


}
