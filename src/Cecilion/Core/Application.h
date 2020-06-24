#pragma once
#include "Window.h"
#include <Event/System_events.h>
#include "Layers/Layer_stack.h"
#include <Event/Event_actor_st.h>

namespace Cecilion {
    class Layer_stack;

    class Application : public Event_actor_st<Window_close_event>, public Layer_stack {
    public:
        Application();
        virtual ~Application() = 0;
        void run();
        void shutdown();
        inline static Application& get() {return *s_instance;}
        inline Window& get_window() {return *this->m_window; }

    private:
        Window* m_window;
        bool m_running = true;
        static Application* s_instance;

    };

    /// This is where the client can define their own starting function.
    Application* CreateApplication();
}
