#include "Inbox_entry_container.h"
#include <Core/Log.h>
void Cecilion::Inbox_entry_container::add_inbox_entry(std::shared_ptr<I_Event_inbox> inbox, Cecilion::I_Event_inbox::Event_callback callback) {
    this->inbox_entry_m.lock();
    this->inbox_entry_list.push_front(new Inbox_entry(std::move(inbox), callback));
    this->inbox_entry_m.unlock();
}

void Cecilion::Inbox_entry_container::delete_entry(Cecilion::I_Event_inbox *p_inbox) {
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

void Cecilion::Inbox_entry_container::dispatch(const std::shared_ptr<Event_message> &event) {
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

Cecilion::Inbox_entry_container::~Inbox_entry_container() {

}
