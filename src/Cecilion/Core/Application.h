#pragma once
#include <memory>

#include "Window.h"
#include "Event/I_Event_actor.h"
namespace Cecilion {
    class Application : public I_Event_actor {
    public:
        Application();
        virtual ~Application();
        void run();
        static void window_close_callback(std::shared_ptr<I_Event_actor> actor, Event_message* message);
        void shutdown();
    private:
        Window* m_window;
        bool m_running = true;
    };

    // To be defined in client
    Application* CreateApplication();
}
