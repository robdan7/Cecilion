#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <Utils/Sparse_set.h>
#include <functional>
#include "System_events.h"

namespace Cecilion {
    using Event_ID_length = uint16_t;

    class I_Event_actor;

    class I_Event_list {

    };

    template<typename Event_type>
    class Event_list : public I_Event_list{
    public:
        /**
         * Return a copy of an event. Not a reference.
         * @param event_ID
         */
        Event_type fetch_event(Event_ID_length event_ID) {
            this->event_m.lock();
            const auto index = this->sparse_set.index(event_ID);
            auto event = this->events.at(index).event;
            if (!(--this->events.at(index).ref_counter)) {
                this->events.at(index) = this->events.back();
                this->events.pop_back();
                this->sparse_set.remove(event_ID);
                this->deleted.push_back(event_ID);
            }
            this->event_m.unlock();
            return event;
        }

        template<typename... Args>
        void send_event(Args&&... args) {
            this->event_m.lock();
            Event_ID_length event_ID;
            if (deleted.size()) {
                event_ID = this->deleted.back();
                this->deleted.pop_back();
            } else {
                event_ID = this->events.size();
            }
            this->sparse_set.put(event_ID);

            this->actor_m.lock();
            this->events.push_back(Event_container{this->function_callbacks.size(), Event_type(args...)});
            this->event_m.unlock();
            // TODO Do callbacks.
            for (auto callback : this->function_callbacks) {
                callback(event_ID);
            }
            this->actor_m.unlock();
        }

        template<typename Callback>
        void subscribe(Callback callback) {
            this->actor_m.lock();
            this->function_callbacks.push_back(callback);
            this->actor_m.unlock();
        }

        /**
         * Let an actor unsubscribe from an event. Deleting an actor without unsubscribing
         * leads to undefined behaviour.
         * @param actor
         */
         // TODO Implement this the proper way, ya dingus fungus.
//        void unsubscribe(I_Event_actor* actor) {
//            this->actor_m.lock();
//            auto iterator = std::find(this->actors.begin(), this->actors.end(), actor);
//            if (iterator != this->actors.end()) {
//                *iterator = this->actors.back();
//                this->actors.pop_back();
//            }
//            this->actor_m.unlock();
//        }

    private:
        struct Event_container {
            uint32_t ref_counter;
            Event_type event;
        };
    private:

        std::mutex event_m, actor_m;
        std::vector<Event_container> events;
        Sparse_set<uint32_t> sparse_set;
        std::vector<uint16_t> deleted;
        // TODO Change this to a struct with Actor ID and callback
        std::vector<std::function<void(uint16_t)>> function_callbacks;
    };

    class Event_system {
    public:

        template<typename Event>
        static Event fetch_event(Event_ID_length event_ID) {
            return static_cast<Event_list <Event>*>(event_containers.at(typeid(Event)))->fetch_event(event_ID);
        }


        template<typename Event>
        static void set_container() {
            if (!event_containers.count(typeid(Event))) {
                event_containers[typeid(Event)] = static_cast<I_Event_list*>(new Event_list<Event>());
            }
        }

        /**
        * Let an event actor subscribe to all of its events.
        * @tparam Events
        * @param actor
        */
//        template<typename... Events>
//        static void subscribe_to(Event_actor<Events...> *actor) {
//            (set_container<Events>(),...);
//            (static_cast<Event_list<Events>*>(event_containers.at(typeid(Events)))->subscribe(static_cast<I_Event_actor*>(actor)), ...);
//        }
        /**
         * Subscribe to one type of event.
         * @tparam Event
         * @tparam Callback - The notify function
         * @param callback
         */
        template<typename Event>
        static void subscribe_to(std::function<void(Event_ID_length)> callback) {
            set_container<Event>();
            static_cast<Event_list<Event>*>(event_containers.at(typeid(Event)))->subscribe(callback);
        }

        template<typename Event, typename... Args>
        static void post(Args&&... args) {
            if (event_containers.contains(typeid(Event))) {
                static_cast<Event_list<Event>*>(event_containers.at(typeid(Event)))->template send_event<Args...>(std::forward<Args>(args)...);
            }
        }



//        template<typename... Events>
//        static void unsubscribe(Event_actor<Events...>* actor) {
//            (static_cast<Event_list<Events>*>(event_containers.at(typeid(Events)))->unsubscribe(
//                    static_cast<I_Event_actor*>(actor)), ...);
//        }
        // TODO Implement unsubscribe to one event type.

    private:


    private:
        static std::unordered_map<std::type_index, I_Event_list*> event_containers;
    };
}