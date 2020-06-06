#include "Application_layer_mt.h"
#include <algorithm>
#include "Layer_stack.h"
//#include <Event/Async_inbox.h>
#include <Core/Log.h>
namespace Cecilion {
//    void Cecilion::Application_layer_mt::dispatch(std::shared_ptr<Event_message> event) {
//
////        std::unordered_map<std::type_index, Inbox_entry*>::iterator it;
//        this->inbox_m.lock();
//        if (this->inbox_entries.count(event->message_ID)) {
////            CORE_LOG_INFO("Arrived at layer");
//            this->inbox_m.unlock();
//            //CORE_LOG_INFO("Invoking inbox");
//            this->inbox_entries[event->message_ID]->invoke_inbox(event);
//        } else if (this->previous != nullptr) {
//            this->inbox_m.unlock();
//            this->previous->dispatch(event);
//        }
//    }

//    void Cecilion::Application_layer_mt::set_previous(Cecilion::Application_layer *previous) {
//        this->previous = previous;
//    }

//    void
//    Cecilion::Application_layer_mt::subscribe_to(std::type_index message_ID, Cecilion::I_Event_inbox::Event_callback callback) {
//        // TODO check for multiple subscriptions to the same event.
//        this->inbox_m.lock();
//        this->inbox_entries[message_ID] = new Inbox_entry(std::shared_ptr<I_Event_inbox>(this->actor_inbox), callback);
//        this->inbox_m.unlock();
//        this->p_parent->add_layer_subscription(message_ID);
//    }

//    void Cecilion::Application_layer_mt::unsubscribe(std::type_index message_ID) {
//        // TODO implement
//        I_Event_actor::unsubscribe(message_ID);
//    }

//    Cecilion::Application_layer_mt::Application_layer_mt() : Application_layer() {
//        this->actor_inbox = std::make_shared<Async_inbox>(std::shared_ptr<I_Event_actor>(this));
//        this->previous = nullptr;
//    }

//    void Cecilion::Application_layer_mt::attach_to_stack(Cecilion::Layer_stack *stack) {
//        this->p_parent = stack;
//    }

//    void Cecilion::Application_layer_mt::detach_from_stack() {
//        this->p_parent = nullptr;
//    }

//    Cecilion::Application_layer_mt::Application_layer_mt(std::shared_ptr<I_Event_inbox> inbox)
//            : Application_layer() {
//        this->actor_inbox = inbox;
//    }
}
