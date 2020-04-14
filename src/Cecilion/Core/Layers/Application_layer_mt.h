#pragma once
#include "Application_layer.h"
#include <Event/Buffered_inbox.h>
namespace Cecilion {
    class Layer_stack;
    class Application_layer_mt : public  Application_layer {
    public:
        Application_layer_mt();

        ~Application_layer_mt() override = default;
        void attach_to_stack(Layer_stack* stack) override;
        void detach_from_stack() override;
        void on_attach() override {};
        void on_detach() override {};
        void on_update() override {};
        void dispatch(std::shared_ptr<Event_message> event) override;
        void set_previous(Application_layer* previous) override;
        void subscribe_to(std::type_index message_ID, I_Event_inbox::Event_callback callback) override;
        void unsubscribe(std::type_index message_ID) override;
    protected:
        /**
         * This is an event actor AND has an event container. I know.
         *
         * The event container is meant to take care of incoming events from the stack,
         * while The actor inbox is sort of in another context. There is also no rule that says the inbox
         * should know what events will trigger certain functions.
         */
        std::unordered_map<std::type_index, Inbox_entry*> inbox_entries;
        std::mutex inbox_m;
        Application_layer* previous;

        Application_layer_mt(std::shared_ptr<I_Event_inbox> inbox);
        Layer_stack* p_parent;
    };
}


