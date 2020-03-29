#include "Application_layer_st.h"
#include <algorithm>
#include "Layer_stack.h"
#include <Core/Log.h>

//void Cecilion::Application_layer_st::dispatch(std::shared_ptr<Event_message> event) {
////        std::list<unsigned int>::iterator it =
////                std::find(this->event_catcher.begin(), this->event_catcher.end(), event->message_ID);
////
////    /// The layer that is highest up in the stack will dispatch the event and block all lower layers.
////    if (it != this->event_catcher.end()) {
////        this->event_container.dispatch(std::make_shared<Event_message>(*event));
////    } else if (this->previous != nullptr) {
////        this->previous->dispatch(event);
////    }
//    std::map<unsigned int, Inbox_entry*>::iterator it;
//        this->inbox_m.lock();
//        if ((it = this->inbox_entries.find(event->message_ID)) != this->inbox_entries.end()) {
//            this->inbox_m.unlock();
//            it->second->invoke_inbox(event);
//        } else if (this->previous != nullptr) {
//            this->inbox_m.unlock();
//            this->previous->dispatch(event);
//        }
//}
//
//void Cecilion::Application_layer_st::set_previous(Cecilion::Application_layer_st *previous) {
//    this->previous = previous;
//}
//
//void
//Cecilion::Application_layer_st::subscribe_to(unsigned int message_ID, Cecilion::I_Event_inbox::Event_callback callback) {
//    // TODO check for multiple subscriptions to the same event.
//    this->inbox_m.lock();
//    this->inbox_entries[message_ID] = new Inbox_entry(std::shared_ptr<I_Event_inbox>(this->actor_inbox), callback);
//    this->inbox_m.unlock();
//    this->p_parent->add_layer_subscription(message_ID);
//}
//
//void Cecilion::Application_layer_st::unsubscribe(unsigned int message_ID) {
//    // TODO implement
//    I_Event_actor::unsubscribe(message_ID);
//}

Cecilion::Application_layer_st::Application_layer_st() : Application_layer_mt(std::make_shared<Buffered_inbox>(std::shared_ptr<I_Event_actor>(this))) {
//    this->actor_inbox = std::make_shared<Buffered_inbox>(std::shared_ptr<I_Event_actor>(this));
//    this->previous = nullptr;
}

void Cecilion::Application_layer_st::on_update() {
    dynamic_cast<Buffered_inbox*>(this->actor_inbox.get())->execute_inbox();
}

//void Cecilion::Application_layer_st::attach_to_stack(Cecilion::Layer_stack *stack) {
//    this->p_parent = stack;
//}
//
//void Cecilion::Application_layer_st::detach_from_stack() {
//    this->p_parent = nullptr;
//}
//
//bool Cecilion::Application_layer_st::has_previous() {
//    return this->previous;
//}
//
//Cecilion::Application_layer_st::Application_layer_st(std::shared_ptr<I_Event_inbox> inbox) : I_Event_actor(""){
//    this->actor_inbox = inbox;
//}
