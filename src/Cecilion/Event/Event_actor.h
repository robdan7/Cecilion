/**
 * @author Robyn
 */
#pragma once
#include "Event_inbox.h"
#include "Event_system.h"
#include <tuple>

namespace Cecilion {
	class I_Event_actor {
	protected:
		I_Event_actor() = default;
		~I_Event_actor() = default;

		template<typename Event>
		Event_inbox<Event_system::Event_ID, Event>* create_inbox() {
			return new Event_inbox<Event_system::Event_ID, Event>(Cecilion::Event_system::execute_event);
		}

		template<typename... Events>
		void unsubscribe(std::tuple<Event_inbox<Event_system::Event_ID, Events>*...>& inboxes) {
			((Event_system::unsubscribe(std::get<Event_inbox<Event_system::Event_ID, Events>*>(inboxes))), ...);

			/// This is a hotfix for unsubscribing. All events that have not been received will run without any script.
			(std::get<Event_inbox<Event_system::Event_ID, Events>*>(inboxes)->set_script([](auto& event) {}), ...);
			((std::get<Event_inbox<Event_system::Event_ID, Events>*>(inboxes)->check_inbox()), ...);
		}

		template<typename... Events>
		void subscribe(std::tuple<Event_inbox<Event_system::Event_ID, Events>*...>& inboxes) {
			((Event_system::subscribe(std::get<Event_inbox<Event_system::Event_ID, Events>*>(inboxes))), ...);
		}

		template<typename Event>
		void subscribe(Event_inbox<Event_system::Event_ID, Event>* inbox) {
			Event_system::subscribe(inbox);
		}
	};

	/**
	 * Generic event actor. Use this as a single threaded event listener. remember to check the inbox periodically.
	 * @tparam Events
	 */
	template<typename... Events>
	class Event_actor : public I_Event_actor {
	protected:
		Event_actor() {
			((std::get<Event_inbox<Event_system::Event_ID, Events>*>(this->m_inboxes) = I_Event_actor::create_inbox<Events>()), ...);
		}

		template<typename Event>
		void set_callback(std::function<void(const Event&)>&& script) {
			auto inbox = std::get<Event_inbox<Event_system::Event_ID, Event>*>(this->m_inboxes);
			inbox->set_script(std::forward<std::function<void(const Event&)>>(script));
			I_Event_actor::subscribe(inbox);
		}

		/**
		 * Check one or moreinboxes
		 * @tparam Event
		 */
		template<typename... Event>
		bool check_inbox() {
			return ((std::get<Event_inbox<Event_system::Event_ID, Event>*>(this->m_inboxes)->check_inbox())||...);
		}

		void on_update() {
			this->check_inbox<Events...>();
		}

		~Event_actor() {
			/// TODO We should probably check if the inboxes have actually subscribed to their events.
			I_Event_actor::unsubscribe<Events...>(this->m_inboxes);

			((delete std::get<Event_inbox<Event_system::Event_ID, Events>*>(this->m_inboxes)), ...);
		}
	private:
		std::tuple<Event_inbox<Event_system::Event_ID, Events>*...> m_inboxes;
	};

	template<typename... Events>
	class Event_actor_obj : public Event_actor<Events...> {
	public:
		Event_actor_obj() {}
		void on_update() {
			Event_actor<Events...>::template check_inbox<Events...>();
		}
		template<typename Event>
		void set_callback_func(std::function<void(const Event&)>&& script) {
			Event_actor<Events...>::template set_callback<Event>(std::forward<std::function<void(const Event&)>>(script));
		}
	};
}
