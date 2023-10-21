/**
 * @author Robyn
 */
#pragma once
#include <unordered_map>
#include <typeindex>
#include <limits>
#include <utility>
#include "Utils/allocator/Sparse_table.h"
#include "Event_inbox.h"
namespace Cecilion {
	/**
	 * This is what we will use to send in-game and system events in the engine. No special sauce is needed to
	 * post an event, and you can even create new types of events on the fly. Just submit any struct or class!
	 */
	class Event_system {
		friend I_Event_actor;
	public:

		Event_system() = default;
		~Event_system() {

            if (s_active_context == this) {
				s_active_context = nullptr;
			}
            for (auto& c : this->m_event_containers) {
                delete c.second;
            }
		}

		using Event_ID = uint16_t;

		template<typename Event, typename... Args>
		static bool post(Args&&... args) {
			// ORVOX_ASSERT(s_active_context, "Could not find an active event system context!");
			if (s_active_context && s_active_context->m_event_containers.count(typeid(Event)) > 0) {
				try {
					static_cast<Event_dispatcher<Event>*>(s_active_context->m_event_containers.at(typeid(Event)))->template post<Args...>(std::forward<Args>(args)...);
				}
				catch (std::exception e) {
					// ORVOX_ERROR("Could not post event of type {0}", typeid(Event).name());
					return false;
				}
				return true;
			}
			else {
				// ORVOX_TRACE("Found no active subscribers for event type {0}", typeid(Event).name());
				return false;
			}
		}

	private:
		class I_Event_dispatcher {
                public:
                    virtual ~I_Event_dispatcher() {}
		};

		template<typename Event_type>
		class Event_dispatcher : public I_Event_dispatcher {
		public:
			Event_dispatcher() = default;
			void subscribe(Event_inbox<Event_ID, Event_type>* inbox) {
				this->m_event_actors.push_back(inbox);
			}

            ~Event_dispatcher() override {}

            /**
             * Post an event to all currently active subscribers.
             * @tparam Args
             * @param args
             */
			template<typename... Args>
			bool post(Args&&... args) {
				this->m_mutex.lock();

				Event_ID event_ID = this->m_event_allocator.alloc(std::piecewise_construct,
					std::forward_as_tuple(this->m_event_actors.size()),
					std::forward_as_tuple(args...));

				this->m_mutex.unlock();
				/// All subscribers need to be notified. Actors that subscribe after an event has been posted
				/// will not be notified.
				for (Event_inbox<Event_ID, Event_type>* inbox : this->m_event_actors) {
					inbox->notify(event_ID);
				}
				return true;
			}

			/**
			 * Call this when an actor wants to execute a script on a given event.
			 * The event will be dropped when all actors that were subscribed at the time have executed it.
			 * @param ID
			 * @param f
			 */
			void execute_event(const Event_ID& ID, const std::function<void(const Event_type&)>& f) {
				/// TODO Minimize the mutex locks.
				this->m_mutex.lock();
				auto& event = this->m_event_allocator[(std::size_t)ID];

				if (event.first == 0) {
					throw std::invalid_argument("Invalid Event ID");
				}
				f(event.second);    /// It should be safe to run this without mutex. The counter has not been decreased yet.
				//std::unique_lock<std::mutex>(this->m_event_mutex);
				event.first--;

				if (event.first == 0) {
					this->m_event_allocator.free(std::forward<const Event_ID&>(ID));
					this->m_deleted_IDs.push_back(ID);
				}
				this->m_mutex.unlock();
			}

			/**
			 * Let an event actor unsubscribe from an event.
			 * @param inbox
			 */
			void unsubscribe(Event_inbox<Event_ID, Event_type>* inbox) {
				this->m_mutex.lock();
				auto iter = std::find(this->m_event_actors.begin(), this->m_event_actors.end(), inbox);
				if (iter != this->m_event_actors.end()) {
					this->m_event_actors.erase(iter);
				}
				this->m_mutex.unlock();
			}

		private:
			Same_arena_allocator<1, uint8_t, std::pair<size_t, Event_type>> m_event_allocator;

			std::mutex m_mutex;
			std::vector<Event_ID> m_deleted_IDs;
			std::vector<Event_inbox<Event_ID, Event_type>*> m_event_actors;
		};

		/**
		 * Subscribe to an event.
		 * @tparam Event
		 * @param f
		 * @return True if the subscription was successful. Returns false if the event inbox is a null ptr.
		 */
		template<typename Event>
		static bool subscribe(Event_inbox<Event_ID, Event>* inbox) {
			// ORVOX_ASSERT(s_active_context, "Could not find an active event system context!");
			if (inbox == nullptr) {
				throw std::invalid_argument("Invalid inbox reference");
			}

			{
				//std::unique_lock<std::mutex>(s_container_mutex);
                s_active_context->s_container_mutex.lock();
				if (s_active_context->m_event_containers.count(typeid(Event)) == 0) {
					s_active_context->m_event_containers[typeid(Event)] = static_cast<I_Event_dispatcher*>(new Event_dispatcher<Event>());
				}
                s_active_context->s_container_mutex.unlock();
			}

			static_cast<Event_dispatcher<Event>*>(s_active_context->m_event_containers.at(typeid(Event)))->subscribe(inbox);

			/// The subscription was successful.
			return true;
			//return static_cast<Event_dispatcher<Event>>(s_event_containers.at(typeid(Event)))
		}

		/**
		 * Let an event actor unsubscribe from one or more events.
		 * @tparam Event
		 * @param inbox
		 * @return
		 */
		template<typename... Event>
		static void unsubscribe(Event_inbox<Event_ID, Event>*... inbox) {
		//	ORVOX_ASSERT(s_active_context, "Could not find an active event system context!");
			([inbox](){
       if (s_active_context->m_event_containers.count(typeid(Event)) > 0) {
        std::cout << "found container" << std::endl;
         static_cast<Event_dispatcher<Event>*>(s_active_context->m_event_containers.at(typeid(Event)))->unsubscribe(inbox);
       } else {
        std::cout << "could not find container" << std::endl;
       }
       }, ...);
		}

		/**
		 * Run the script on a given event id.
		 * @tparam Event - The event type
		 * @param ID - The event ID (only unique among the same type of events)
		 * @param f - Script function.
		 */
		template<typename Event>
		static void execute_event(const Event_ID& ID, const std::function<void(const Event&)>& script) {
			// ORVOX_ASSERT(s_active_context, "Could not find an active event system context!");
			static_cast<Event_dispatcher<Event>*>(s_active_context->m_event_containers.at(typeid(Event)))->execute_event(ID, script);
		}

  public:
	  void set_as_active_context() {
			s_active_context = this;
		}

	private:
		std::unordered_map<std::type_index, I_Event_dispatcher*> m_event_containers;
		static Event_system* s_active_context;
		std::mutex s_container_mutex;
	};
}
