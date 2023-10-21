#pragma once
#include <Event/System_events.h>
#include "Layers/Layer_stack.h"

namespace Cecilion {
    class Layer_stack;

    class Application : public Event_actor_obj<Cecilion::Events::Window_close_event>, public Layer_stack {
    public:
        Application();
        virtual ~Application() = 0;
        void run();
        void shutdown();
        inline static Application& get() {return *s_instance;}

    private:
        //Window* m_window;
        bool m_running = true;
        static Application* s_instance;

    };

    /// This is where the client can define their own starting function.
    Application* CreateApplication();
}
