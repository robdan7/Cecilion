#pragma once

//#include <Event/I_Event_actor.h>
#include <Event/Event_actor.h>
#include <typeindex>
namespace Cecilion {
    template<typename... Events>
    class Layer_stack;

    /**
     * The application layer is sort of an event actor inside a layer stack.
     * Events will be dispatched
     * @tparam Events
     */
    template<typename... Events>
    class Application_layer : public Event_actor<Events...>  {
    public:
        Application_layer() {

        }

        virtual void detach_from_stack() = 0;
        virtual void on_attach() = 0;
        virtual void on_detach() = 0;
        virtual void on_update() = 0;
        virtual void on_imgui_render() {}

        template<typename Event>
        void notify_event(unsigned int ID) {
            this->template notify<Event>(ID);
        }
//        virtual void dispatch(std::shared_ptr<Event_message> event) = 0;
        virtual void set_previous(Application_layer* previous) = 0;
//        virtual void subscribe_to(std::type_index message_ID, I_Event_inbox::Event_callback callback) = 0;
        virtual void unsubscribe(std::type_index message_ID) = 0;
//        void attach_to_stack(Layer_stack<Events...>* stack) {
//            // This does not do anything right now. Don't think id need to either.
//        }
    };
}
