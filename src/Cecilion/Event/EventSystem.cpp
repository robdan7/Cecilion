#include "EventSystem.h"
#include "../Core/Log.h"
#include <iostream>
#include "../Utils/Semaphore.h"
#include "EventSystem.h"
#include <condition_variable>
#include <mutex>
using namespace Cecilion;

class ActorEntry {
public:
    I_EventActor* actor;
    I_EventActor::EventCallback ActorCallback;
    std::vector<EventMessage*> MessageStack;
    std::mutex RunEventMutex, MessageStackMutex;
    std::thread EventThread;



    ActorEntry(I_EventActor *MessageActor, I_EventActor::EventCallback MessageCallback) {
        this->actor = MessageActor;
        this->ActorCallback = MessageCallback;
    }

    /**
     * Create and start an event dispatching thread. The thread will stay alive as long as
     * there are events curently in the queue. To shut down the thread, simply clear the message stack.
     * @param MessageActor
     */
    void StartThread(I_EventActor* MessageActor, I_EventActor::EventCallback MessageCallback) {
        std::mutex* MutexPointer = &this->RunEventMutex;

        std::vector<EventMessage*>* EventStackPointer = &this->MessageStack;

        // Create the thread that takes care of all the event handling.
        this->EventThread = std::thread ( [MutexPointer, MessageActor, EventStackPointer, MessageCallback] () {
            while(true) { // Run until all events are dispatched.
                // Lock the mutex and keep it locked until the next iteration.
                std::unique_lock<std::mutex> lock(*MutexPointer);

                if (EventStackPointer->size() == 0) {
                    // We cant use the size() as loop condition since the mutex must be locked
                    // when we check if the event stack is empty.
                    break;
                }

                EventMessage* message = EventStackPointer->back();
                EventStackPointer->pop_back();
                lock.unlock();  // Unlock the mutex to avoid deadlock and what not.
                MessageCallback(MessageActor,message);
            }
        });
    }

    ~ActorEntry() {
        this->RunEventMutex.lock();
        // Clearing the message stack will terminate the event thread if it is running.
        this->MessageStack.clear();
        this->RunEventMutex.unlock();


        if (this->EventThread.joinable()) {
            // Let the thread finish if it needs to run. It will quit immediately
            // because it's outside the loop and the message stack
            this->EventThread.join();
        }
    }

    void callback(EventMessage *message) {
        this->MessageStackMutex.lock();
        if (this->MessageStack.size() == 0) {
            // When the mutex is locked and the stack is empty, it means that
            // The thread has stopped.
            if (this->EventThread.joinable()) {
                // Let the thread finish if it needs to run. It will quit immediately
                // because it's outside the loop and the message stack
                this->EventThread.join();
            }
            this->StartThread(this->actor, this->ActorCallback);
        }
        this->MessageStack.push_back(message);
        this->MessageStackMutex.unlock();
        this->EventThread.detach();
    }
};



class ActorEntryContainer {
    list<ActorEntry*> actors;
    std::mutex listMutex;
    unsigned int eventID;
    public: ActorEntryContainer(unsigned int eventID) {
        this->eventID = eventID;
    }
    ~ActorEntryContainer() {

    }
    void addActor(I_EventActor* actor, Cecilion::I_EventActor::EventCallback callback) {
        this->listMutex.lock();
        this->actors.push_front(new ActorEntry(actor, callback));
        this->listMutex.unlock();
    }

    void deleteActor(I_EventActor* actor) {

       this->listMutex.lock();
        ActorEntry* searchResult = nullptr;
       for (ActorEntry* entry : this->actors) {
            if (entry->actor == actor) {
                searchResult = entry;
            }
       }
       if (searchResult != nullptr) {
           this->actors.remove(searchResult);
       } else {
           CORE_LOG_ERROR(" Event actor " + actor->name() + "is not a subscriber to event ID " + std::to_string(this->eventID));
       }
//        this->actors->remove(actor);
        this->listMutex.unlock();
    }

    void dispatch(EventMessage* message) {
        this->listMutex.lock();
        for (ActorEntry* entry : this->actors) {
            //entry->callback(message);
            // TODO check why is it terminating call without exception.
//            std::thread th([entry, message] () {
//                entry->callback(message);
//            });
            entry->callback(message);
        }
        this->listMutex.unlock();
    }
};


list<Cecilion::EventMessage*> MessageStack;
map<unsigned int, ActorEntryContainer*>* ActorList;


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

void Cecilion::EventSystem::Init() {
    initialized = true;
    MessageStack = list<Cecilion::EventMessage*>();
    ActorList = new map<unsigned int, ActorEntryContainer*>();
    CORE_LOG_INFO("Event system up and running");
}

/**
 * Subscribe to an event. When the event is posted a callback function will be called with the subscriber and
 * the message as a reference.
 *
 * One actor can have multiple entries for the same event, but the messaging system will always assume there
 * is only one entry per actor.
 * @param messageID
 * @param actor
 * @param callback
 */
void Cecilion::EventSystem::subscribe(unsigned int messageID, Cecilion::I_EventActor* actor, I_EventActor::EventCallback callback) {
    CheckInit();
    ActorListMutex.lock();
    auto* actors = new ActorEntryContainer(messageID);
    if (ActorList->find(messageID) == ActorList->end()) {
        //actors.push_front(new ActorEntry(actor, callback));
        //actors->push_front(new ActorEntry(actor, callback));
        (*ActorList)[messageID] = actors;
    } else {
        actors = (*ActorList)[messageID];
    }
    actors->addActor(actor,callback);
    ActorListMutex.unlock();
    CORE_LOG_TRACE("Actor " + actor->name() + "subscribed to message ID " + std::to_string(messageID));
}

void EventSystem::post(EventMessage *message) {
    CheckInit();
    dispatchPost(message);
}

void EventSystem::dispatchPost(EventMessage *message) {
    ActorListMutex.lock();
    if (ActorList->count(message->messageID) > 0) {
        CORE_LOG_TRACE("Event ID " + std::to_string(message->messageID) + " has been posted");
        ActorList->at(message->messageID)->dispatch(message);
    } else {
        CORE_LOG_WARN("No actor is currently listening to message ID " + std::to_string(message->messageID));
    }
    ActorListMutex.unlock();
}

void EventSystem::unsubscribe(unsigned int messageID, Cecilion::I_EventActor *actor) {
    CheckInit();
    if (ActorList->count(messageID) == 0) {
        CORE_LOG_ERROR("Delete exception. No actors are subscribed to " + std::to_string(messageID));
        return;
    }
    ActorList->at(messageID)->deleteActor(actor);
}



