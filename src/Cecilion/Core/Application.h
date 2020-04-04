#pragma once
#include <memory>

#include "Window.h"
#include "Event/I_Event_actor.h"
#include "Layers/Layer_stack.h"
#include <ImGui/ImGui_layer.h>
#include <Platform/OpenGL/GL_shader.h>
#include <Platform/OpenGL/GL_buffer.h>
#include <Renderer/Buffer.h>
#include <Renderer/Vertex_array.h>
namespace Cecilion {
    class Application : public I_Event_actor {
    public:
        Application();
        virtual ~Application();
        void run();
        static void window_close_callback(std::shared_ptr<I_Event_actor> actor, Event_message* message);
        void shutdown();
        inline static Application& get() {return *s_instance;}
        inline Window& get_window() {return *this->m_window; }
        void push_layer(const std::shared_ptr<Application_layer>& layer);
        void pop_layer(const std::shared_ptr<Application_layer>& layer);
        void push_overlay(const std::shared_ptr<Application_layer>& layer);
        void pop_overlay(const std::shared_ptr<Application_layer>& layer);
    private:
        std::shared_ptr<ImGui_layer> m_imgui_layer;
        Window* m_window;
        bool m_running = true;
        Layer_stack* application_layers;
        static Application* s_instance;
        std::unique_ptr<GL_shader> m_shader;
//        std::shared_ptr<Vertex_buffer> m_vertex_buffer;
//        std::shared_ptr<Index_buffer> m_index_buffer;
        std::shared_ptr<Vertex_array> m_vertex_array;
    };

    // To be defined in client
    Application* CreateApplication();
}
