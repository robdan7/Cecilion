#pragma once

#include <functional>
#include <atomic>
#include <mutex>
#include <bitset>
#include "Event_system.h"
#include <iostream>

namespace Cecilion {


    template<typename... Events>
    class Inbox {
    private:
        /**
        * Container for one type of events in an inbox. It will repeatedly call the lambda function
        * for all events that are currently in the inbox list.
        * @tparam E
        */
        template<typename Event>
        class test_event_container {
        public:

            /** Typedef for the callback type **/
            typedef std::function<void(Event event)> My_callback;

            test_event_container() {

            }

            void Init(int index) {
                this->index = index;
            }

            /**
             * This is the method that actually calls the callback given by an
             * actor.
             */
            void check_inbox_container() {
                if (!this->incoming_events.size()) return;
                /// Iterate through the list and call the function with all events.

                for (auto event : this->incoming_events) {
//                    Event_system::fetch_event<Event>(event);

                    /** This fetches a copy of the event from the event system
                     *  and dispatches the callback function.
                     **/
                    this->callback(Event_system::fetch_event<Event>(event));
                }
                this->incoming_events.clear();
            }

            /**
             * Add an event ID to the list of new events.
             * @param event_ID
             */
            void notify(uint16_t event_ID) {
                this->incoming_events.push_back(event_ID);
            }

            template<typename Function>
            void set_callback(Function function) {
                this->callback = function;
            }

            void lock_mutex() {
                this->event_m.lock();
            }

            void unlock_mutex() {
                this->event_m.unlock();
            }

        public:
            int index;
            My_callback callback = [](Event i) {};
            std::vector<uint16_t> incoming_events;
            std::mutex event_m;
        };

    public:
        Inbox() {
            int i = 0;
            ((std::get<test_event_container<Events>*>(this->my_tuple) = new test_event_container<Events>()),...);
//            (std::get<test_event_container<Events>>(this->my_tuple)->Init(i++), ...);
        }
        /**
    * In the multi threaded version:
    * Here goes some function that iterates through all things in the tuple until all
    * lists are empty. The thread will then go to sleep
    */

        /**
         * Check all events in the inbox once. It looks weird, I know.
         * Think of Events as a list of variable types, and check_container is called
         * for every type in Events.
         */
        void check_inbox() {
            (this->check_container<Events>(),...);
        }

        /**
         * Just notify the event list that some event has arrived.
         * @tparam Event - The event type.
         * @param event_ID - Global event ID used by the event system.
         */
        template<typename Event>
        void notify(unsigned int event_ID) {
            auto container = std::get<test_event_container<Event>*>(this->my_tuple);
            this->has_events_m.lock();
            this->has_events.set(container->index);
            container->lock_mutex();
            this->has_events_m.unlock();
            container->notify(event_ID);
            container->unlock_mutex();
        }

        /**
         * Set the callback function for one type of subscribed events.
         * @tparam Event
         * @tparam Function
         * @param function
         */
        template<typename Event, typename Function>
        void set_callback(Function function) {
            std::get<test_event_container<Event>*>(my_tuple)->set_callback(function);
        }

    private:
        /**
         * This locks a container in the inbox and checks its events.
         * @tparam Event - The event type to check.
         */
        template<typename Event>
        void check_container() {
            auto container = std::get<test_event_container<Event>*>(this->my_tuple);
            this->has_events_m.lock();
            this->has_events.set(container->index, 0);
            container->lock_mutex();
            this->has_events_m.unlock();
            container->check_inbox_container();
            container->unlock_mutex();
        }
    private:
        std::tuple<test_event_container<Events>*...> my_tuple;
        std::mutex has_events_m;
        std::bitset<sizeof...(Events)> has_events;
    };


    class I_Event_actor {
    };

    /**
     * This is the generic class for event actors. Both single threaded and
     * multi threaded actors inherit from this class.
     * @tparam Events
     */
    template<typename... Events>
    class Event_actor : public I_Event_actor {
    public:

        Event_actor() {
            this->inbox = new Inbox<Events...>();
        }

        ~Event_actor() {
            delete this->inbox;
        }

        /**
         * This is the only method the user need to care about. It sets
         * a callback for an event type, so whenever that event is generated
         * the callback is executed. Remember that you can capture the actor object inside
         * a lambda function.
         * @tparam Event - Any kind of object, actually. This is the event that was posted.
         * @param function - Presumably a lambda function that takes the event as argument.
         */
        template<typename Event>
        void set_callback(std::function<void(Event)> function) {
            this->inbox->template set_callback<Event, std::function<void(Event)>>(function);
        }

        /**
         * Check the actor inbox.
         */
        void check_inbox() {
            this->inbox->check_inbox();
        }

    protected:
        /**
         * Subscriptions are not added when the actor is created. Instead,
         * the user must decide if and when to initialize all subscriptions.
         */
        void init_subscriptions() {
            (Event_system::subscribe_to<Events>([this](auto ID){
                this->inbox->template notify<Events>(ID);
            }),...);
        }

        /**
         * Internal manual notify method. Undefined behaviour if subscriptions are
         * initialized.
         * @tparam Event
         * @param ID
         */
        template<typename Event>
        void notify(unsigned int ID) {
            this->inbox->template notify<Event>(ID);
        }
    private:
        Inbox<Events...> *inbox;
    };
}
