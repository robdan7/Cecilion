#pragma once

#include <Event/I_Event_actor.h>

namespace Cecilion {

    /**
     * Interface class for Application layers.
     */
    class Layer_stack;
    class Application_layer : public I_Event_actor {
    public:
        Application_layer() : I_Event_actor("") {}
        virtual void attach_to_stack(Layer_stack* stack) = 0;
        virtual void detach_from_stack() = 0;
        virtual void on_attach() = 0;
        virtual void on_detach() = 0;
        virtual void on_update() = 0;
        virtual void on_imgui_render() {}
        virtual void dispatch(std::shared_ptr<Event_message> event) = 0;
        virtual void set_previous(Application_layer* previous) = 0;
        virtual void subscribe_to(std::type_index message_ID, I_Event_inbox::Event_callback callback) = 0;
        virtual void unsubscribe(std::type_index message_ID) = 0;
    };
}
