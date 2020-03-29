#pragma once
#include "I_Event_inbox.h"
#include <mutex>
namespace Cecilion {
    class Inbox_entry_container {
    public:
        ~Inbox_entry_container();
        void add_inbox_entry(std::shared_ptr<I_Event_inbox> inbox, I_Event_inbox::Event_callback callback);
        void delete_entry(I_Event_inbox*  p_inbox);
        void dispatch(const std::shared_ptr<Event_message>& event);
    protected:
        std::list<Inbox_entry*> inbox_entry_list;
        std::mutex inbox_entry_m;
    };

}
