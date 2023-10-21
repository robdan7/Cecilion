/**
 * @author Robyn
 */
#pragma once
#include <functional>
#include <mutex>
namespace Cecilion {
	class Event_system;
	class I_Event_actor;
	template<typename Event_ID, typename Event_type>
	class Event_inbox {
		friend Event_system;
		friend I_Event_actor;
	private:
		Event_inbox(void(*func)(const Event_ID& ID, const std::function<void(const Event_type&)>&)) {
			this->p_fetch_event = func;
		}
	public:
		void set_script(std::function<void(const Event_type&)>&& f) {
			this->p_script = f;
		}

		/**
		 * This executes all the events in the inbox.
		 */
        bool check_inbox() {
            this->m_event_mutex.lock();
            if (this->m_incoming_events.size() == 0) {
                this->m_event_mutex.unlock();
                return false;
            }
            std::vector<Event_ID> event_copy = std::vector<Event_ID>();
            event_copy.assign(this->m_incoming_events.begin(), this->m_incoming_events.end());
            this->m_incoming_events.clear();
            this->m_event_mutex.unlock();

            for (const auto &ID : event_copy) {
                this->p_fetch_event(ID, this->p_script);
            }
            return true;
        }

		void notify(const Event_ID& ID) {
			this->m_event_mutex.lock();
			this->m_incoming_events.push_back(ID);
			this->m_event_mutex.unlock();
		}
	private:
		std::vector<Event_ID> m_incoming_events;
		std::function<void(const Event_type&)> p_script = [](const Event_type& event) {};
		std::mutex m_event_mutex;
		void(*p_fetch_event)(const Event_ID& ID, const std::function<void(const Event_type&)>&);
	};
}
