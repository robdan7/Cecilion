#pragma once
#include <memory>

#include "Window.h"
#include "Layers/Layer_stack.h"
#include <functional>
#include <Event/Event_actor_st.h>

//#include <ImGui/ImGui_layer.h>
#include <Platform/OpenGL/GL_shader.h>
#include <Platform/OpenGL/GL_buffer.h>
#include <Renderer/Buffer.h>
#include <Renderer/Vertex_array.h>
namespace Cecilion {
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
