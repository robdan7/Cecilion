#include "Event_system.h"
//#include "../Core/Log.h"

#include <mutex>
#include <vector>
#include <thread>
#include "Log.h"

using namespace Cecilion;

class Actor_entry {
public:
    I_Event_actor* p_actor;
    I_Event_actor::Event_callback actor_callback;
    std::vector<Event_message*> message_stack;
    std::mutex event_m, message_stack_m;
    std::thread event_thread;



    Actor_entry(I_Event_actor *MessageActor, I_Event_actor::Event_callback MessageCallback) {
        this->p_actor = MessageActor;
        this->actor_callback = MessageCallback;
    }

    /**
     * Create and start an event dispatching thread. The thread will stay alive as long as
     * there are events curently in the queue. To shut down the thread, simply clear the message stack.
     * @param message_actor
     */
    void StartThread(I_Event_actor* message_actor, I_Event_actor::Event_callback message_callback) {
        std::mutex* p_mutex_pointer = &this->event_m;

        std::vector<Event_message*>* p_event_stack = &this->message_stack;

        // Create the thread that takes care of all the event handling.
        this->event_thread = std::thread ([p_mutex_pointer, message_actor, p_event_stack, message_callback] () {
            while(true) { // Run until all events are dispatched.
                // Lock the mutex and keep it locked until the next iteration.
                std::unique_lock<std::mutex> lock(*p_mutex_pointer);

                if (p_event_stack->size() == 0) {
                    // We cant use the size() as loop condition since the mutex must be locked
                    // when we check if the event stack is empty.
                    break;
                }

                Event_message* message = p_event_stack->back();
                p_event_stack->pop_back();
                lock.unlock();  // Unlock the mutex to avoid deadlock and what not.
                message_callback(message_actor, message);
            }
        });
    }

    ~Actor_entry() {
        this->event_m.lock();
        // Clearing the message stack will terminate the event thread if it is running.
        this->message_stack.clear();
        this->event_m.unlock();


        if (this->event_thread.joinable()) {
            // Let the thread finish if it needs to run. It will quit immediately
            // because it's outside the loop and the message stack
            this->event_thread.join();
        }
    }

    void callback(Event_message *message) {
        this->message_stack_m.lock();
        if (this->message_stack.size() == 0) {
            // When the mutex is locked and the stack is empty, it means that
            // The thread has stopped.
            if (this->event_thread.joinable()) {
                // Let the thread finish if it needs to run. It will quit immediately
                // because it's outside the loop and the message stack
                this->event_thread.join();
            }
            this->StartThread(this->p_actor, this->actor_callback);
        }
        this->message_stack.push_back(message);
        this->message_stack_m.unlock();
        this->event_thread.detach();
    }
};



class ActorEntryContainer {
    list<Actor_entry*> actors;
    std::mutex actor_lock_m;
    unsigned int eventID;
    public: ActorEntryContainer(unsigned int eventID) {
        this->eventID = eventID;
    }
    ~ActorEntryContainer() {

    }
    void addActor(I_Event_actor* actor, Cecilion::I_Event_actor::Event_callback callback) {
        this->actor_lock_m.lock();
        this->actors.push_front(new Actor_entry(actor, callback));
        this->actor_lock_m.unlock();
    }

    void deleteActor(I_Event_actor* actor) {

       this->actor_lock_m.lock();
        Actor_entry* search_result = nullptr;
       for (Actor_entry* entry : this->actors) {
            if (entry->p_actor == actor) {
                search_result = entry;
            }
       }
       if (search_result != nullptr) {
           this->actors.remove(search_result);
       } else {
           CORE_LOG_ERROR(" Event actor " + actor->name() + "is not a subscriber to event ID " + std::to_string(this->eventID));
       }
//        this->actors->remove(actor);
        this->actor_lock_m.unlock();
    }

    void dispatch(Event_message* message) {
        this->actor_lock_m.lock();
        for (Actor_entry* entry : this->actors) {
            //entry->callback(message);
            // TODO check why is it terminating call without exception.
//            std::thread th([entry, message] () {
//                entry->callback(message);
//            });
            entry->callback(message);
        }
        this->actor_lock_m.unlock();
    }
};


list<Cecilion::Event_message*> message_stack;
map<unsigned int, ActorEntryContainer*>* actor_list;


std::mutex ActorListMutex;
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
    message_stack = list<Cecilion::Event_message*>();
    actor_list = new map<unsigned int, ActorEntryContainer*>();
    CORE_LOG_INFO("Event system up and running");
}

/**
 * Subscribe to an event. When the event is posted a callback function will be called with the subscriber and
 * the message as a reference.
 *
 * One actor can have multiple entries for the same event, but the messaging system will always assume there
 * is only one entry per actor.
 * @param message_ID
 * @param actor
 * @param callback
 */
void Cecilion::Event_system::subscribe(unsigned int message_ID, Cecilion::I_Event_actor* actor, I_Event_actor::Event_callback callback) {
    CheckInit();
    ActorListMutex.lock();
    auto* actors = new ActorEntryContainer(message_ID);
    if (actor_list->find(message_ID) == actor_list->end()) {
        //actors.push_front(new ActorEntry(actor, callback));
        //actors->push_front(new ActorEntry(actor, callback));
        (*actor_list)[message_ID] = actors;
    } else {
        actors = (*actor_list)[message_ID];
    }
    actors->addActor(actor,callback);
    ActorListMutex.unlock();
    CORE_LOG_TRACE("Actor " + actor->name() + "subscribed to message ID " + std::to_string(message_ID));
}

void Event_system::post(Event_message *message) {
    CheckInit();
    dispatch_event(message);
}

void Event_system::dispatch_event(Event_message *message) {
    ActorListMutex.lock();
    if (actor_list->count(message->c_message_ID) > 0) {
        CORE_LOG_TRACE("Event ID " + std::to_string(message->c_message_ID) + " has been posted");
        actor_list->at(message->c_message_ID)->dispatch(message);
    } else {
        CORE_LOG_WARN("No actor is currently listening to message ID  " + std::to_string(message->c_message_ID));
    }
    ActorListMutex.unlock();
}

void Event_system::unsubscribe(unsigned int message_ID, Cecilion::I_Event_actor *actor) {
    CheckInit();
    if (actor_list->count(message_ID) == 0) {
        CORE_LOG_ERROR("Delete exception. No actors are subscribed to " + std::to_string(message_ID));
        return;
    }
    actor_list->at(message_ID)->deleteActor(actor);
}



