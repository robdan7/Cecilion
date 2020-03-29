#pragma once
#include "Application_layer_mt.h"
#include <Event/Buffered_inbox.h>
namespace Cecilion {
    class Layer_stack;
    class Application_layer_st : public Application_layer_mt {
    public:
        Application_layer_st();

//        ~Application_layer_st() override = default;
//        void attach_to_stack(Layer_stack* stack);
//        void detach_from_stack();
//        virtual void on_attach() {};
//        virtual void on_detach() {};
        void on_update() override;
//        void push_layer(const std::shared_ptr<Application_layer_st>& layer, bool overlay);
//        void push_overlay();
//        void pop_layer(const std::shared_ptr<Application_layer_st>& layer, bool overlay);
//        void dispatch(std::shared_ptr<Event_message> event);
//        void set_previous(Application_layer_st* previous);
//        void subscribe_to(unsigned int message_ID, I_Event_inbox::Event_callback callback) override;
//        void unsubscribe(unsigned int message_ID) override;
//        bool has_previous();

//    private:
//        /**
//         * This is an event actor AND has an event container. I know.
//         *
//         * The event container is meant to take care of incoming events from the stack,
//         * while The actor inbox is sort of in another context. There is also no rule that says the inbox
//         * should know what events will trigger certain functions.
//         */
//        std::map<unsigned int, Inbox_entry*> inbox_entries;
//        std::mutex inbox_m;
//        Application_layer_st* previous;
//    protected:
//        Application_layer_st(std::shared_ptr<I_Event_inbox> inbox);
//        Layer_stack* p_parent;

    };
}

